#include "pch.h"
#include "PatchTexture.h"
#include "Utility.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite3.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"

PatchTexture::~PatchTexture() = default;
PatchTexture::PatchTexture() = default;
PatchTexture::PatchTexture(TextureSlice texSlice) noexcept :
	m_texSlice{ texSlice }
{}

PatchTexture::PatchTexture(const PatchTexture& other) noexcept :
	UIComponent{ other },
	m_texSlice{ other.m_texSlice }
{}

UIComponent* PatchTexture::GetCenterComponent() noexcept
{
	if (auto tex = static_cast<PatchTexture*>(GetChildComponent(1)); tex)
		return tex->GetCenterComponent();
	return nullptr;
}

bool PatchTexture::ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept
{
	ReturnIfFalse(ForEach([&sizes](PatchTexture* tex, size_t index) {
		return tex->ChangeSize(sizes[index]);
		}));
	return true;
}

bool PatchTexture::ApplyPositions(DirectionType dirType, const XMUINT2& size, const vector<XMUINT2>& sizes) noexcept
{
	vector<XMINT2> positions = ExtractStartPosFromSizes(dirType, sizes);
	ReturnIfFalse(ForEach([this, &size, &positions](PatchTexture*, size_t index) {
		return ChangePosition(index, size, positions[index]);
		}));
	return true;
}

bool PatchTexture::ApplySizeAndPosition(DirectionType dirType, const XMUINT2& size, const vector<UIComponent*>& components) noexcept
{
	const vector<XMUINT2>& sizes = StretchSize(dirType, size, components);
	ReturnIfFalse(ApplyStretchSize(sizes));
	ReturnIfFalse(ApplyPositions(dirType, size, sizes));
	ApplySize(size);

	return true;
}

//�� Ŭ������ �´� ������ �������̽� �Լ��� ��Ӿ��� �������� �ϰ� ������ ó���ϴ� �Լ��ε� �̰� utility��
//�ø��� UIComponent���� �θ� �ٸ����� �������̽��� ���� �Լ����� �������� ó���� �� �ִ�.
template<typename FuncT>
decltype(auto) PatchTexture::GetSourceByType(UIComponent* component, FuncT&& Func) const noexcept
{
	switch (GetTypeID()) {
	case ComponentID::PatchTextureLite1: return Func(static_cast<PatchTextureLite1*>(component));
	case ComponentID::PatchTextureLite3: return Func(static_cast<PatchTextureLite3*>(component));
	case ComponentID::PatchTextureStd1: return Func(static_cast<PatchTextureStd1*>(component));
	case ComponentID::PatchTextureStd3: return Func(static_cast<PatchTextureStd3*>(component));
	default: return decltype(Func(declval<PatchTextureLite1*>())){}; //�⺻Ÿ���� ���������� �⺻Ÿ�� ���� Rectangle�� �����ϴϱ� �̰� Rectangle{}�� ����.
	}
}

vector<Rectangle> PatchTexture::GetChildSourceList() const noexcept
{
	const auto& components = GetChildComponents();
	vector<Rectangle> results;

	for (UIComponent* child : components)
		results.push_back(GetSourceByType(child, [](auto* c) { return c->GetSource(); }));

	return results;
}

bool PatchTexture::ImplementChangeSize(const XMUINT2& size) noexcept
{
	auto dirType = TextureSliceToDirType(*m_texSlice);
	ReturnIfFalse(dirType);
	ReturnIfFalse(IsBiggerThanSource(*dirType, size, GetChildSourceList()));

	return ApplySizeAndPosition(*dirType, size, GetChildComponents());
}

static void AccumulateSize(DirectionType dir, const XMUINT2& size, XMUINT2& totalSize) noexcept {
	switch (dir) {
	case DirectionType::Horizontal:
		totalSize.x += size.x;
		totalSize.y = max(totalSize.y, size.y);
		break;
	case DirectionType::Vertical:
		totalSize.y += size.y;
		totalSize.x = max(totalSize.x, size.x);
		break;
	}
}

bool PatchTexture::FitToTextureSource() noexcept
{
	auto dirType = TextureSliceToDirType(*m_texSlice);
	ReturnIfFalse(dirType);

	XMUINT2 totalSize{};
	vector<XMUINT2> individualSizes{};
	auto allFitted = ForEach([dirType, &totalSize, &individualSizes](PatchTexture* tex, size_t) {
		ReturnIfFalse(tex->FitToTextureSource());
		const XMUINT2 size = GetSizeFromRectangle(tex->GetArea());
		individualSizes.emplace_back(size);
		AccumulateSize(*dirType, size, totalSize);
		return true;
		});
	ReturnIfFalse(allFitted); //allFitted ������ �Ἥ ReturnIfFalse�� �� ������ ���ٿ� �����(�극��ũ ������)�� �ȵǱ� �����̴�.
	ReturnIfFalse(ApplyPositions(*dirType, totalSize, individualSizes));

	SetSize(totalSize);
	return true;
}

bool PatchTexture::ForEach(predicate<PatchTexture*, size_t> auto&& Each)
{
	const auto& components = GetChildComponents();
	size_t size = components.size();
	ReturnIfFalse(size == 3);

	for (size_t n : views::iota(0u, size))
		if (!Each(static_cast<PatchTexture*>(components[n]), n)) return false;

	return true;
}