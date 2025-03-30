#include "pch.h"
#include "ImageGrid1.h"
#include "../../../Include/IRenderer.h"
#include "../../Utility.h"
#include "../../Config.h"
#include "../JsonOperation.h"
#include "../TextureSourceBinder/TextureSourceBinder.h"

ImageGrid1::~ImageGrid1() = default;
ImageGrid1::ImageGrid1() : 
	m_texController{ nullptr }
{}

ImageGrid1::ImageGrid1(const ImageGrid1& o) :
	ImageGrid{ o },
	m_bindKey{ o.m_bindKey },
	m_sourceIndex{ o.m_sourceIndex },
	m_texController{ o.m_texController },
	m_index{ o.m_index },
	m_filename{ o.m_filename },
	m_source{ o.m_source },
	m_gfxOffset{ o.m_gfxOffset }
{}

unique_ptr<UIComponent> ImageGrid1::CreateClone() const
{
	return unique_ptr<ImageGrid1>(new ImageGrid1(*this));
}

bool ImageGrid1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const ImageGrid1* o = static_cast<const ImageGrid1*>(&rhs);

	auto result = tie(m_bindKey, m_sourceIndex) == tie(o->m_bindKey, o->m_sourceIndex);
	assert(result);

	return result;
}

void ImageGrid1::SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept
{
	m_filename = sourceInfo.filename;
	m_index = sourceInfo.GetIndex();
	m_source = sourceInfo.GetSource(m_sourceIndex);
	if (auto gfxOffset = sourceInfo.GetGfxOffset(); gfxOffset)
		m_gfxOffset = *gfxOffset;
	m_texController = texController;

	if (GetSize() == XMUINT2{}) //사이즈가 없다면 source 사이즈로 초기화 한다.
		SetSize(RectangleToXMUINT2(m_source));
}

bool ImageGrid1::ImplementBindSourceInfo(TextureSourceBinder* sourceBinder, ITextureController*) noexcept
{
	if (m_bindKey.empty()) return false;
	auto sourceInfoRef = sourceBinder->GetTextureSourceInfo(m_bindKey);
	ReturnIfFalse(sourceInfoRef);

	const auto& srcInfo = sourceInfoRef->get();
	SetSourceInfo(srcInfo, nullptr);
	
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
	if (!m_index) return nullopt;
	return m_texController->GetTextureAreaList(*m_index, PackRGBA(255, 255, 255, 0));
}

void ImageGrid1::ImplementRender(ITextureRender* render) const
{
	const auto& position = GetPosition();
	const auto& size = GetSize();
	Rectangle destination(position.x, position.y, size.x, size.y);

	RECT source = RectangleToRect(m_source);
	render->Render(*m_index, destination, &source);
}

bool ImageGrid1::Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept
{
	if (bindKey.empty()) return false;
	SetLayout(layout);
	m_bindKey = bindKey;
	m_sourceIndex = sourceIndex;

	return true;
}

void ImageGrid1::SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept
{
	m_index = index;
	m_source = source[0];
}

void ImageGrid1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("BindKey", m_bindKey);
	operation.Process("SourceIndex", m_sourceIndex);
}

unique_ptr<ImageGrid1> CreateImageGrid1(const UILayout& layout, const string& bindKey, size_t sourceIndex)
{
	auto grid1 = make_unique<ImageGrid1>();
	return CreateIfSetup(move(grid1), layout, bindKey, sourceIndex);
}