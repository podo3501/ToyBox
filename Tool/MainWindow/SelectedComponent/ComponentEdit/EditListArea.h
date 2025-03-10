#pragma once
#include "EditWindow.h"

class ListArea;
class EditListArea : public EditWindow
{
public:
	~EditListArea();
	EditListArea() = delete;
	EditListArea(ListArea* listArea, IRenderer* renderer, CommandList* cmdList) noexcept;

private:
	ListArea* m_listArea;
};