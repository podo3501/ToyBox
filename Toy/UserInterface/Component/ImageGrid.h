#pragma once
#include "../UIComponent.h"

struct ITextureController;
class ImageGrid : public UIComponent
{
public:
	~ImageGrid();
	ImageGrid();
	virtual bool SetupFromData(const XMUINT2&, size_t, const vector<Rectangle>&) { return true; }
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept {}

protected:
	ImageGrid(const ImageGrid& other);
};
