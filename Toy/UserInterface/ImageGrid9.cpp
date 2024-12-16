#include "pch.h"
#include "ImageGrid9.h"
#include "ImageGrid3.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"

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
