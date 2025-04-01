#pragma once
#include "../UIComponent.h"

struct ITextureController;
class ImageGrid : public UIComponent
{
public:
	~ImageGrid();
	ImageGrid();
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept {}

protected:
	ImageGrid(const ImageGrid& other);
};
