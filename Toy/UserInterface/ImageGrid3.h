#pragma once
#include "UIComponent.h"

struct ImageSource;
class ImageGrid1;

class ImageGrid3 : public UIComponent
{
public:
	virtual bool ChangeArea(const Rectangle& area) noexcept override;

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);

private:
};

unique_ptr<ImageGrid3> CreateImageGrid3(const string& name, const UILayout& layout, const ImageSource& source);