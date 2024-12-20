#pragma once
#include "UIComponent.h"

struct ImageSource;

class ImageGrid9 : public UIComponent
{
protected:
	ImageGrid9(const ImageGrid9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~ImageGrid9();
	ImageGrid9();

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);
	bool ChangeArea(const Rectangle& area) noexcept;

private:
};
