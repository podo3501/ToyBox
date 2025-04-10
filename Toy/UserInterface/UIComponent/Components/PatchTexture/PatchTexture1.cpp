#include "pch.h"
#include "PatchTexture1.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "../../../JsonOperation/JsonOperation.h"
#include "../../../TextureResourceBinder/TextureResourceBinder.h"

PatchTexture1::~PatchTexture1() = default;
PatchTexture1::PatchTexture1() : 
	PatchTexture{ TextureSlice::One },
	m_texController{ nullptr }
{}

PatchTexture1::PatchTexture1(const PatchTexture1& o) :
	PatchTexture{ o },
	m_bindKey{ o.m_bindKey },
	m_sourceIndex{ o.m_sourceIndex },
	m_texController{ o.m_texController },
	m_index{ o.m_index },
	m_filename{ o.m_filename },
	m_source{ o.m_source },
	m_gfxOffset{ o.m_gfxOffset }
{}

unique_ptr<UIComponent> PatchTexture1::CreateClone() const
{
	return unique_ptr<PatchTexture1>(new PatchTexture1(*this));
}

bool PatchTexture1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTexture1* o = static_cast<const PatchTexture1*>(&rhs);

	auto result = tie(m_bindKey, m_sourceIndex) == tie(o->m_bindKey, o->m_sourceIndex);
	assert(result);

	return result;
}

void PatchTexture1::SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept
{
	m_filename = sourceInfo.filename;
	m_index = sourceInfo.GetIndex();
	m_source = sourceInfo.GetSource(m_sourceIndex);
	if (auto gfxOffset = sourceInfo.GetGfxOffset(); gfxOffset)
		m_gfxOffset = *gfxOffset;
	m_texController = texController;

	if (GetSize() == XMUINT2{}) //사이즈가 없다면 source 사이즈로 초기화 한다.
		SetSize(GetSizeFromRectangle(m_source));
}

bool PatchTexture1::Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept
{
	if (bindKey.empty()) return false;
	SetLayout(layout);
	m_bindKey = bindKey;
	m_sourceIndex = sourceIndex;

	return true;
}

bool PatchTexture1::ImplementBindSourceInfo(TextureResourceBinder* resBinder, ITextureController*) noexcept
{
	if (m_bindKey.empty()) return false;
	auto sourceInfoRef = resBinder->GetTextureSourceInfo(m_bindKey);
	ReturnIfFalse(sourceInfoRef);

	const auto& srcInfo = sourceInfoRef->get();
	SetSourceInfo(srcInfo, nullptr);

	return true;
}

void PatchTexture1::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
	ChangeBindKeyWithIndex(key, sourceInfo, 0);
}

void PatchTexture1::ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
{
	m_bindKey = key;
	m_sourceIndex = sourceIndex;
	SetSourceInfo(sourceInfo, nullptr);
}

static inline UINT32 PackRGBA(UINT8 r, UINT8 g, UINT8 b, UINT8 a)
{
	return (static_cast<UINT32>(a) << 24) |
		(static_cast<UINT32>(b) << 16) |
		(static_cast<UINT32>(g) << 8) |
		(static_cast<UINT32>(r));
}

optional<vector<Rectangle>> PatchTexture1::GetTextureAreaList()
{	
	if (!m_index) return nullopt;
	return m_texController->GetTextureAreaList(*m_index, PackRGBA(255, 255, 255, 0));
}

void PatchTexture1::ImplementRender(ITextureRender* render) const
{
	const auto& position = GetPosition();
	const auto& size = GetSize();
	Rectangle destination(position.x, position.y, size.x, size.y);

	RECT source = RectangleToRect(m_source);
	render->Render(*m_index, destination, &source);
}

void PatchTexture1::SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept
{
	m_index = index;
	m_source = source[0];
}

bool PatchTexture1::FitToTextureSource() noexcept
{
	if (m_source.IsEmpty()) return false;

	SetSize(GetSizeFromRectangle(m_source));
	return true;
}

void PatchTexture1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("BindKey", m_bindKey);
	operation.Process("SourceIndex", m_sourceIndex);
}

unique_ptr<PatchTexture1> CreatePatchTexture1(const UILayout& layout, const string& bindKey, size_t sourceIndex)
{
	auto patchTex1 = make_unique<PatchTexture1>();
	return CreateIfSetup(move(patchTex1), layout, bindKey, sourceIndex);
}