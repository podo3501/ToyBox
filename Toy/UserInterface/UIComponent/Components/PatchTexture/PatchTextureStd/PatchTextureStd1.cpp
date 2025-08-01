#include "pch.h"
#include "PatchTextureStd1.h"
#include "../Include/IRenderer.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/JsonOperation/JsonOperation.h"

PatchTextureStd1::~PatchTextureStd1() = default;
PatchTextureStd1::PatchTextureStd1() : 
	PatchTextureStd{ TextureSlice::One },
	m_texController{ nullptr }
{
	m_coord.SetOwner(this);
}

PatchTextureStd1::PatchTextureStd1(const PatchTextureStd1& o) :
	PatchTextureStd{ o },
	m_bindKey{ o.m_bindKey },
	m_sourceIndex{ o.m_sourceIndex },
	m_texController{ o.m_texController },
	m_filename{ o.m_filename },
	m_gfxOffset{ o.m_gfxOffset },
	m_coord{ o.m_coord }
{
	m_coord.SetOwner(this);
}

unique_ptr<UIComponent> PatchTextureStd1::CreateClone() const
{
	return unique_ptr<PatchTextureStd1>(new PatchTextureStd1(*this));
}

bool PatchTextureStd1::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const PatchTextureStd1* o = static_cast<const PatchTextureStd1*>(&rhs);

	auto result = (tie(m_bindKey, m_sourceIndex) == tie(o->m_bindKey, o->m_sourceIndex));
	Assert(result);

	return result;
}

void PatchTextureStd1::SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept
{
	m_filename = sourceInfo.filename;
	m_coord.SetIndexedSource(*sourceInfo.GetIndex(), { sourceInfo.GetSource(m_sourceIndex) });
	if (auto gfxOffset = sourceInfo.GetGfxOffset(); gfxOffset)
		m_gfxOffset = *gfxOffset;
	m_texController = texController;

	if (GetSize() == XMUINT2{}) //사이즈가 없다면 source 사이즈로 초기화 한다.
		FitToTextureSource();
}

bool PatchTextureStd1::Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept
{
	if (bindKey.empty()) return false;
	SetLayout(layout);
	m_bindKey = bindKey;
	m_sourceIndex = sourceIndex;

	return true;
}

bool PatchTextureStd1::ImplementBindSourceInfo(TextureResourceBinder* resBinder, ITextureController*) noexcept
{
	if (m_bindKey.empty()) return false; 
	auto sourceInfoRef = resBinder->GetTextureSourceInfo(m_bindKey);
	ReturnIfFalse(sourceInfoRef);

	const auto& srcInfo = sourceInfoRef->get();
	SetSourceInfo(srcInfo, nullptr);

	return true;
}

void PatchTextureStd1::ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept
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

optional<vector<Rectangle>> PatchTextureStd1::GetTextureAreaList()
{	
	if (auto index = m_coord.GetIndex(); index)
		return m_texController->GetTextureAreaList(*index, PackRGBA(255, 255, 255, 0));
	return nullopt;
}

void PatchTextureStd1::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("BindKey", m_bindKey);
	operation.Process("SourceIndex", m_sourceIndex);
}
