#include "pch.h"
#include "ImageGrid3.h"
#include "ImageGrid1.h"
#include "../Utility.h"
#include "UIUtility.h"
#include "UIType.h"
#include "UILayout.h"

static bool ValidateInput(const string& name, const ImageSource& source)
{
    if (name.empty()) return false;
    if (source.filename.empty()) return false;
    if (source.list.size() != 3) return false;

    return true;
}

static unique_ptr<ImageGrid1> CreateImageGrid1(const string& baseName, size_t idx, 
    const ImageSource& source, const PositionRectangle& posRect)
{
    const auto& grid1name = baseName + "_" + to_string(idx);
    UILayout grid1layout(posRect.area, Origin::LeftTop);

    ImageSource imgSource{ source.filename, { source.list[idx] } };

    auto grid1 = make_unique<ImageGrid1>();
    grid1->SetImage(grid1name, grid1layout, imgSource);

    return grid1;
}

bool ImageGrid3::SetImage(const string& name, const UILayout& layout, const ImageSource& source, bool yStretched)
{
    ReturnIfFalse(ValidateInput(name, source));

	SetName(name);
	SetLayout(layout);

    vector<PositionRectangle> posRects = StretchSize(StretchType::Width, layout.GetArea(), source.list, yStretched);
    for (std::size_t idx = 0; idx < source.list.size(); ++idx) 
    {
        auto grid1 = CreateImageGrid1(name, idx, source, posRects[idx]);
        AddComponent(move(grid1), posRects[idx].pos);	//rectangle이 아니라 XMINT로 해야 하지 않나? x, y 값은 언제나 0이다.
    }

	return true;
}