#pragma once
#include "EditWindow.h"

class Container;

class EditContainer : public EditWindow
{
public:
	~EditContainer();
	EditContainer() = delete;
	EditContainer(Container* container, CommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	Container* m_container;
};