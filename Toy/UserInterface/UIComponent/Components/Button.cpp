#include "pch.h"
#include "Button.h"
#include "../../JsonOperation/JsonOperation.h"
#include "../../../Utility.h"

//�� Ŭ������ ���忡�� �ϴ°� ����. container�� �� ���� �� ��������. ���Ŀ� �� Ŭ������ ��� ���� ���Ѻ���.
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

bool Button::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(m_container->ChangeSize(size));
	return UIComponent::ImplementChangeSize(size);
}

//holdToKeepPressed�� ���߿� �ɼ��� �����ϴ� ������ �ٲ���. ��ư�� �پ��� ��ư���� �����ϱ� ����.
bool Button::Setup(const UILayout& layout, unique_ptr<UIComponent> container) noexcept
{
	SetLayout(layout);

	m_container = container.get();
	//m_container->SetStateFlag(StateFlag::Attach | StateFlag::Detach, false);
	UIEx(this).AttachComponent(move(container), {});

	return true;
}

void Button::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<Button> CreateButton(const UILayout& layout, unique_ptr<UIComponent> container)
{
	unique_ptr<Button> button = make_unique<Button>();
	return CreateIfSetup(move(button), layout, move(container));
}