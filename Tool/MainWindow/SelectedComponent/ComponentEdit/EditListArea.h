#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class ListArea;
class EditListArea : public EditWindow
{
public:
	~EditListArea();
	EditListArea() = delete;
	EditListArea(ListArea* listArea, UICommandList* cmdList) noexcept;

private:
	ListArea* m_listArea;
};