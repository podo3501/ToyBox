#pragma once
#include "EditWindow.h"

class TextureSourceBinder;
class Container;
class EditContainer : public EditWindow
{
public:
	~EditContainer();
	EditContainer() = delete;
	EditContainer(Container* container, TextureSourceBinder* sourceBinder, CommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	Container* m_container;
};