#pragma once
#include "../UIComponent.h"

struct ITextureController;
class ImageGrid : public UIComponent
{
public:
	~ImageGrid();
	ImageGrid();
	virtual bool FitToTextureSource() noexcept = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	
protected:
	ImageGrid(const ImageGrid& other);
};
