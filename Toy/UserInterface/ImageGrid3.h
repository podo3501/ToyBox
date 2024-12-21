#pragma once
#include "UIComponent.h"

struct ImageSource;
class ImageGrid1;

class ImageGrid3 : public UIComponent
{
protected:
	ImageGrid3(const ImageGrid3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~ImageGrid3();
	ImageGrid3();

	virtual void ChangeSize(const XMUINT2& size) noexcept override;


	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);
	Rectangle GetFirstComponentSource() const noexcept;

private:
};

unique_ptr<ImageGrid3> CreateImageGrid3(const string& name, const UILayout& layout, const ImageSource& source);