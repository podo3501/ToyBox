#pragma once
#include "EditWindow.h"

class TextureSourceBinder;
class ListArea;
class EditListArea : public EditWindow
{
public:
	~EditListArea();
	EditListArea() = delete;
	EditListArea(ListArea* listArea, IRenderer* renderer, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept;

private:
	ListArea* m_listArea;
};