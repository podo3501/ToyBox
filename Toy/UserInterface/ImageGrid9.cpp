#include "pch.h"
#include "ImageGrid9.h"
#include "ImageGrid3.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"

ImageGrid9::~ImageGrid9() = default;
ImageGrid9::ImageGrid9() = default;

ImageGrid9::ImageGrid9(const ImageGrid9& o) :
	UIComponent{ o }
{
}

unique_ptr<UIComponent> ImageGrid9::CreateClone() const
{
	return unique_ptr<ImageGrid9>(new ImageGrid9(*this));
}

static bool ValidateInput(const string& name, const ImageSource& source)
{
	if (name.empty()) return false;
	if (source.filename.empty()) return false;
	if (source.list.size() != 9) return false;

	return true;
}

static inline vector<Rectangle> ExtractSourceRects(const ImageSource& source)
{
	return { source.list[0], source.list[3], source.list[6] };
}

static std::unique_ptr<ImageGrid3> CreateImageGrid3(const string& name, size_t idx,
	const ImageSource& source, const PositionRectangle& posRect)
{
	const auto& grid3name = name + "_" + std::to_string(idx);
	UILayout grid3layout(posRect.area, Origin::LeftTop);

	ImageSource imgSource
	{
		source.filename,
		{ source.list[idx * 3], source.list[idx * 3 + 1], source.list[idx * 3 + 2] }
	};

	auto grid3 = std::make_unique<ImageGrid3>();
	grid3->SetImage(grid3name, grid3layout, imgSource);

	return grid3;
}

bool ImageGrid9::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
	ReturnIfFalse(ValidateInput(name, source));

	SetName(name);
	SetLayout(layout);

	auto srcHList = ExtractSourceRects(source);
	auto posRects = StretchSize(StretchType::Height, layout.GetArea(), srcHList);
	for (size_t idx = 0; idx < srcHList.size(); ++idx)
	{
		auto grid3 = CreateImageGrid3(name, idx, source, posRects[idx]);
		AddComponent(move(grid3), posRects[idx].pos);
	}

	return true;
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

bool ImageGrid9::ChangeArea(const Rectangle& area) noexcept
{
	const vector<UIComponent*> components = GetComponents();
	vector<Rectangle> list = GetSourceList(components);
	vector<PositionRectangle> posRects = StretchSize(StretchType::Height, area, list);

	for (int idx{ 0 }; idx < components.size(); ++idx)
	{
		ChangePosition(idx, posRects[idx].pos);
		components[idx]->ChangeArea(posRects[idx].area);
	}
	UIComponent::ChangeArea(area);

	return true;
}
