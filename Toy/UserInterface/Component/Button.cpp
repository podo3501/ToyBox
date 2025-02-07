#include "pch.h"
#include "Button.h"
#include "../JsonOperation.h"
#include "Container.h"

Button::~Button() = default;
Button::Button() :
	m_container{ nullptr }
{}

Button::Button(const Button& o) :
	UIComponent{ o }
{
	ReloadDatas();
}

void Button::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_container = componentList[0];
}

unique_ptr<UIComponent> Button::CreateClone() const
{
	return unique_ptr<Button>(new Button(*this));
}

void Button::ChangeSize(const XMUINT2& size) noexcept
{
	m_container->ChangeSize(size);
	UIComponent::ChangeSize(size);
}

bool Button::Setup(const UILayout& layout, unique_ptr<UIComponent>&& container) noexcept
{
	SetLayout(layout);

	container->SetAttachmentState(AttachmentState::Attach);
	m_container = container.get();
	UIEx(this).AttachComponent(move(container), {});

	SetAttachmentState(AttachmentState::Detach);

	return true;
}

void Button::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<Button> CreateButton(const UILayout& layout, unique_ptr<Container>&& container)
{
	unique_ptr<Button> button = make_unique<Button>();
	if(!button->Setup(layout, move(container))) return nullptr;

	return button;
}