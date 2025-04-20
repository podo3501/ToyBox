#pragma once
#include "EditWindow.h"

class TextureResourceBinder;
class Container;
class EditContainer : public EditWindow
{
public:
	~EditContainer();
	EditContainer() = delete;
	EditContainer(Container* container, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	Container* m_container;
};