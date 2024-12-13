#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"

bool ImageGrid3::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
	if (name.empty()) return false;
	if (source.filename.empty()) return false;
	if (source.list.size() != 3) return false;

	SetName(name);
	SetLayout(layout);

	vector<PositionRectangle> posRects = StretchSize(layout.GetArea(), source.list);
	for (int idx{ 0 }; const auto & src : source.list)
	{
		const string& grid1name = name + "_" + to_string(idx);
		unique_ptr<ImageGrid1> grid1 = make_unique<ImageGrid1>();

		const PositionRectangle& posRect = posRects.at(idx);
		UILayout grid1layout(posRect.area, Origin::LeftTop);
		ImageSource imgSource;
		imgSource.filename = source.filename;
		imgSource.list.push_back(src);

		grid1->SetImage(grid1name, grid1layout, imgSource);
		AddComponent(move(grid1), posRect.pos);	//rectangle이 아니라 XMINT로 해야 하지 않나? x, y 값은 언제나 0이다.
		++idx;
	}

	return true;
}