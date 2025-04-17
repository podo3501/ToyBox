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
	m_withoutBindKey{ o.m_withoutBindKey },
	m_sourceIndex{ o.m_sourceIndex },
	m_texController{ o.m_texController },
	m_filename{ o.m_filename },
	m_gfxOffset{ o.m_gfxOffset },
	m_coord{ o.m_coord }
{}

unique_ptr<UIComponent> PatchTexture1::CreateClone() const
{
	return unique_ptr<PatchTexture1>(new PatchTexture1(*this));
}

bool PatchTexture1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTexture1* o = static_cast<const PatchTexture1*>(&rhs);

	auto result = tie(m_bindKey, m_withoutBindKey, m_sourceIndex) == 
		tie(o->m_bindKey, o->m_withoutBindKey, o->m_sourceIndex);
	Assert(result);

	return result;
}

void PatchTexture1::SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept
{
	m_filename = sourceInfo.filename;
	SetIndexedSource(*sourceInfo.GetIndex(), { sourceInfo.GetSource(m_sourceIndex) });
	if (auto gfxOffset = sourceInfo.GetGfxOffset(); gfxOffset)
		m_gfxOffset = *gfxOffset;
	m_texController = texController;

	if (GetSize() == XMUINT2{}) //사이즈가 없다면 source 사이즈로 초기화 한다.
		FitToTextureSource();
}

bool PatchTexture1::Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept
{
	if (bindKey.empty()) return false;
	SetLayout(layout);
	m_bindKey = bindKey;
	m_sourceIndex = sourceIndex;

	return true;
}

bool PatchTexture1::SetupWithoutBindKey(const UILayout& layout) noexcept
{
	SetLayout(layout);
	m_withoutBindKey = true;
	return true;
}

bool PatchTexture1::ImplementBindSourceInfo(TextureResourceBinder* resBinder, ITextureController*) noexcept
{
	if (m_withoutBindKey) return true;//texture switcher일때에는 bindKey를 사용하지 않는다.
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
	if (auto index = m_coord.GetIndex(); index)
		return m_texController->GetTextureAreaList(*index, PackRGBA(255, 255, 255, 0));
	return nullopt;
}

void PatchTexture1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("BindKey", m_bindKey);
	operation.Process("WithoutBindKey", m_withoutBindKey);
	operation.Process("SourceIndex", m_sourceIndex);
}

unique_ptr<PatchTexture1> CreatePatchTexture1(const UILayout& layout, const string& bindKey, size_t sourceIndex)
{
	auto patchTex1 = make_unique<PatchTexture1>();
	return CreateIfSetup(move(patchTex1), layout, bindKey, sourceIndex);
}

unique_ptr<PatchTexture1> CreatePatchTexture1Lite(const UILayout& layout)
{
	auto patchTex1 = make_unique<PatchTexture1>();
	if (!patchTex1->SetupWithoutBindKey(layout)) return nullptr;

	return move(patchTex1);
}