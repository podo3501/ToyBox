#include "pch.h"
#include "ImageGrid1.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../../Config.h"
#include "../JsonOperation.h"

ImageGrid1::~ImageGrid1()
{
	Release();
}

void ImageGrid1::Release() noexcept
{
	if (m_texController && m_index)
	{
		m_texController->ReleaseTexture(*m_index);
		m_texController = nullptr;
		m_gfxOffset = {};
		m_index = nullopt;
	}
}

ImageGrid1::ImageGrid1() : 
	m_texController{ nullptr }
{}

ImageGrid1::ImageGrid1(const ImageGrid1& o) :
	UIComponent{ o },
	m_texController{ o.m_texController }
{
	m_index = o.m_index;
	m_filename = o.m_filename;
	m_source = o.m_source;
}

void ImageGrid1::AddRef() const noexcept
{
	if(m_texController && m_index)
		m_texController->AddRef(*m_index);
}

unique_ptr<UIComponent> ImageGrid1::CreateClone() const
{
	auto clone = unique_ptr<ImageGrid1>(new ImageGrid1(*this));
	clone->AddRef();
	return clone;
}

bool ImageGrid1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const ImageGrid1* o = static_cast<const ImageGrid1*>(&rhs);

	auto result = tie(m_filename, m_source) == tie(o->m_filename, o->m_source);
	assert(result);

	return result;
}

bool ImageGrid1::ImplementLoadResource(ITextureLoad* load)
{
	Release();

	XMUINT2 texSize{};
	size_t index{ 0 };
	ReturnIfFalse(load->LoadTexture(GetResourceFullFilename(m_filename), index, &texSize, &m_gfxOffset));

	m_index = index;

	if (GetSize() == XMUINT2{} && m_source == Rectangle{}) //파일이름만 셋팅하면 크기 및 그려지는 부분은 전체로 설정한다.
	{
		SetSize(texSize);
		m_source = { 0, 0, static_cast<long>(texSize.x), static_cast<long>(texSize.y) };
	}

	return true;
}

bool ImageGrid1::ImplementPostLoaded(ITextureController* texController)
{
	m_texController = texController;
	return true;
}

static inline UINT32 PackRGBA(UINT8 r, UINT8 g, UINT8 b, UINT8 a)
{
	return (static_cast<UINT32>(a) << 24) |
		(static_cast<UINT32>(b) << 16) |
		(static_cast<UINT32>(g) << 8) |
		(static_cast<UINT32>(r));
}

optional<vector<Rectangle>> ImageGrid1::GetTextureAreaList()
{
	 return m_texController->GetTextureAreaList(GetResourceFullFilename(m_filename), PackRGBA(255, 255, 255, 0));
}

void ImageGrid1::ImplementRender(ITextureRender* render) const
{
	const auto& position = GetPosition();
	const auto& size = GetSize();
	Rectangle destination(position.x, position.y, size.x, size.y);

	RECT source = RectangleToRect(m_source);
	render->Render(*m_index, destination, &source);
}

bool ImageGrid1::SetImage(const UILayout& layout, const ImageSource& source) noexcept
{
	if (source.filename.empty()) return false;
	if (source.list.size() != 1) return false;

	SetLayout(layout);

	m_filename = source.filename;
	m_source = source.list.at(0);

	return true;
}

void ImageGrid1::SetFilenameToLoadInfo(const wstring& filename) noexcept
{
	m_filename = filename;
}

void ImageGrid1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("Filename", m_filename);
	operation.Process("Source", m_source);
}

unique_ptr<ImageGrid1> CreateImageGrid1(const UILayout& layout, const ImageSource& source)
{
	auto grid1 = make_unique<ImageGrid1>();
	return grid1->SetImage(layout, source) ? move(grid1) : nullptr;
}