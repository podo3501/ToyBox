#include "pch.h"
#include "ImageGrid9.h"
#include "ImageGrid3.h"
#include "../../../Utility.h"
#include "../UIUtility.h"
#include "ImageGridHelper.hpp"

ImageGrid9::~ImageGrid9() = default;
ImageGrid9::ImageGrid9() = default;
ImageGrid9::ImageGrid9(const ImageGrid9& o) :
	UIComponent{ o }
{}

unique_ptr<UIComponent> ImageGrid9::CreateClone() const
{
	return unique_ptr<ImageGrid9>(new ImageGrid9(*this));
}

bool ImageGrid9::Setup(const UILayout& layout, const string& bindKey)
{
	SetLayout(layout);

	vector<optional<StateFlag::Type>> stateFlags{ StateFlag::Y_SizeLocked, nullopt, StateFlag::Y_SizeLocked };
	for (size_t idx : views::iota(0, 3))
	{
		auto grid3 = CreateImageGrid3(DirectionType::Horizontal, { {}, Origin::LeftTop }, bindKey, idx);
		if (auto flag = stateFlags[idx]; flag) grid3->SetStateFlag(*flag, true);
		UIEx(this).AttachComponent(move(grid3), {});
	}
	SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);

	return true;
}

bool ImageGrid9::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	ChangeSize(GetSize(), true);
	UpdatePositionsManually();

	return true;
}

const string& ImageGrid9::GetBindKey() const noexcept
{
	ImageGrid3* imgGrid3 = ComponentCast<ImageGrid3*>(GetChildComponent(0));
	return imgGrid3->GetBindKey();
}

void ImageGrid9::ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept
{
	for (size_t index : views::iota(0u, 3u))
	{
		ImageGrid3* imgGrid3 = ComponentCast<ImageGrid3*>(GetChildComponent(index));
		imgGrid3->ChangeBindKey(key, sourceInfo, index);
	}
}

static vector<Rectangle> GetSourceList(const vector<UIComponent*>& components) noexcept
{
	vector<Rectangle> srcList;
	ranges::transform(components, back_inserter(srcList), [](auto component) {
		ImageGrid3* image = ComponentCast<ImageGrid3*>(component);
		return image->GetFirstComponentSource();
		});
	return srcList;
}

bool ImageGrid9::ImplementChangeSize(const XMUINT2& size) noexcept
{
	const vector<UIComponent*> components = GetChildComponents();
	vector<Rectangle> list = GetSourceList(components);
	ReturnIfFalse(IsBiggerThanSource(DirectionType::Vertical, size, list));

	vector<XMUINT2> sizes = StretchSize(DirectionType::Vertical, size, components);
	vector<XMINT2> positions = ExtractStartPosFromSizes(DirectionType::Vertical, sizes);
	for (auto idx : views::iota(0u, components.size()))
	{
		ChangePosition(idx, size, positions[idx]);
		ReturnIfFalse(components[idx]->ChangeSize(sizes[idx]));
	}
	ApplySize(size);

	return true;
}

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const string& bindKey)
{
	auto img9 = make_unique<ImageGrid9>();
	return CreateIfSetup(move(img9), layout, bindKey);
}