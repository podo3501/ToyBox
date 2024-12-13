#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"
//1, 1, 3
bool ImageGrid3::SetImage(const string& name, const UILayout& layout, const ImageSource& source)
{
	if (name.empty()) return false;
	if (source.filename.empty()) return false;
	if (source.list.size() != 3) return false;

	SetName(name);
	SetLayout(layout);

	vector<PositionRectangle> posRects = StretchSize(layout.GetArea(), source.list);
    for (std::size_t idx = 0; idx < source.list.size(); ++idx) 
    {
        const auto& grid1name = name + "_" + std::to_string(idx);
        const auto& posRect = posRects.at(idx);
        UILayout grid1layout(posRect.area, Origin::LeftTop);

        ImageSource imgSource;
        imgSource.filename = source.filename;
        imgSource.list.push_back(source.list[idx]);

        auto grid1 = std::make_unique<ImageGrid1>();
        grid1->SetImage(grid1name, grid1layout, imgSource);
        AddComponent(move(grid1), posRect.pos);	//rectangle�� �ƴ϶� XMINT�� �ؾ� ���� �ʳ�? x, y ���� ������ 0�̴�.
    }

	return true;
}