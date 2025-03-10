#pragma once
#include "../UIComponent.h"

struct ITextureController;
class ImageGrid : public UIComponent
{
public:
	~ImageGrid();
	ImageGrid();
	virtual bool SetupFromData(const XMUINT2&, const IndexedSource&) { return true; }
	virtual void SetIndexedSource(const IndexedSource&) noexcept {}

protected:
	ImageGrid(const ImageGrid& other);
};
