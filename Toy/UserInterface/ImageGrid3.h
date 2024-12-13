#pragma once
#include "UIComponent.h"

struct ImageSource;

class ImageGrid3 : public UIComponent
{
public:
	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);

private:

};
