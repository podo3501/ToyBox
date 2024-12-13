#include "pch.h"
#include "ImageGrid9.h"
#include "ImageGrid3.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"

bool ImageGrid9::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
	if (name.empty()) return false;
	if (source.filename.empty()) return false;
	if (source.list.size() != 9) return false;

	SetName(name);
	SetLayout(layout);

	vector<PositionRectangle> posRects = StretchSize(layout.GetArea(), source.list);
	for (std::size_t idx = 0; idx < source.list.size(); idx += 3)
	{
		const auto& grid3name = name + "_" + std::to_string(idx);
		const auto& posRect = posRects.at(idx % 3);
		UILayout grid3layout(posRect.area, Origin::LeftTop);

		ImageSource imgSource;
		imgSource.filename = source.filename;
		imgSource.list.push_back(source.list[idx + 0]);
		imgSource.list.push_back(source.list[idx + 1]);
		imgSource.list.push_back(source.list[idx + 2]);

		auto grid3 = std::make_unique<ImageGrid3>();
		grid3->SetImage(grid3name, grid3layout, imgSource);
		AddComponent(move(grid3), posRect.pos);
	}

	return true;
}
