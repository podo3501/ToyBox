#include "pch.h"
#include "UIComponent.h"
#include "UILayout.h"
#include "UIType.h"
#include "../Utility.h"
#include "../HelperClass.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "JsonOperation.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_name{},
	m_layout{ make_unique<UILayout>(Rectangle{}, Origin::LeftTop) }
{}

UIComponent::UIComponent(const string& name, const Rectangle& rect) :
	m_name{ name },
	m_layout{ make_unique<UILayout>(rect, Origin::LeftTop) }
{}

UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_layout = make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& transCom) {
		return move(transCom.Clone());
		//return make_unique<TransformComponent>(*transComp.get());
		});
}

//UIComponent& UIComponent::operator=(const UIComponent& other)
//{
//	if (this == &other) return *this;
//
//	m_name = other.m_name;
//	m_layout = std::make_unique<UILayout>(*other.m_layout);
//	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& transCom) {
//		return move(transCom);
//		//return TransformComponent(transCom);
//		//return make_unique<TransformComponent>(*transComp.get());
//		});
//
//	return *this;
//}

string UIComponent::GetType() const { return string(typeid(*this).name()); }

//unique_ptr<UIComponent> UIComponent::Clone() 
//{
//	return make_unique<UIComponent>(*this);
//}

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetType() != o.GetType()) return false;

	ReturnIfFalse(tie(m_name) == tie(o.m_name));
	ReturnIfFalse(CompareUniquePtr(m_layout, o.m_layout));
	ReturnIfFalse(m_components == o.m_components);

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_components{ move(o.m_components) }
{}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& transComp) {
		return transComp.m_component->LoadResources(load);
		});
}

const TransformComponent* UIComponent::FindTransformComponent(const string& name) const noexcept
{
	auto find = ranges::find_if(m_components, [&name](const auto& transComp) {
		return transComp.m_component->GetName() == name;
		});

	if (find == m_components.end()) return nullptr;
	return &(*find);
}

void UIComponent::SetSelected(bool selected) noexcept
{
	m_selected = selected;
}

bool UIComponent::GetSelected() const noexcept
{
	return m_selected;
}

bool UIComponent::SetDatas(IGetValue* value)
{
	return ranges::all_of(m_components, [value](const auto& transCom) {
		return transCom.m_component->SetDatas(value);
		});
}

bool UIComponent::ProcessUpdate(const XMINT2& position, InputManager* inputManager) noexcept
{
	if (!m_enable) return true;

	Update(position, inputManager);

	return ranges::all_of(m_components, [this, &position, inputManager](const auto& transCom) {
		const auto& curPosition = m_layout->GetPosition(transCom.m_position) + position;
		if (inputManager)
			inputManager->GetMouse()->SetOffset(curPosition);
		return transCom.m_component->ProcessUpdate(curPosition, inputManager);
		});
}

void UIComponent::ProcessRender(IRender* render)
{
	if (!m_enable) 
		return;

	Render(render);

	ranges::for_each(m_components, [render](const auto& transCom) {
		transCom.m_component->ProcessRender(render);
		});
}

const Rectangle& UIComponent::GetArea() const noexcept
{
	return m_layout->GetArea();
}

void UIComponent::SetChildPosition(const string& name, const Vector2& position) noexcept
{
	auto transComponent = const_cast<TransformComponent*>(FindTransformComponent(name));
	if (transComponent == nullptr) return;

	transComponent->m_position = position;
}

UIComponent* UIComponent::GetComponent(const string& name) const noexcept
{
	auto transComponent = FindTransformComponent(name);
	if (transComponent == nullptr) return nullptr;

	return transComponent->m_component.get();
}

vector<UIComponent*> UIComponent::GetComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_components, back_inserter(componentList), [](const auto& transCom) {
		return transCom.m_component.get();
		});

	return componentList;
}

bool UIComponent::IsHover(const XMINT2& pos) const noexcept
{
	if (m_layout->IsArea(pos)) return true;

	return ranges::any_of(m_components, [this, &pos](const auto& transCom) {
		const auto& curPosition = pos - m_layout->GetPosition(transCom.m_position);
		return transCom.m_component->IsHover(curPosition);
		});
}

bool UIComponent::IsArea(const XMINT2& pos) const noexcept
{
	return m_layout->IsArea(pos);
}

void UIComponent::GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept
{
	if (m_layout->IsArea(pos))
		outList.push_back(this);

	ranges::for_each(m_components, [this, &pos, &outList](auto& transCom) {
		const auto& curPosition = pos - m_layout->GetPosition(transCom.m_position);
		transCom.m_component->GetComponents(curPosition, outList);
		});
}

void UIComponent::SetSize(const XMUINT2& size)
{
	m_layout->Set({ 0, 0, static_cast<long>(size.x), static_cast<long>(size.y) });
}

XMUINT2 UIComponent::GetSize() const noexcept
{
	const auto& area = m_layout->GetArea();
	return { static_cast<uint32_t>(area.width - area.x), static_cast<uint32_t>(area.height - area.y) };
}

bool UIComponent::ChangeArea(const Rectangle& area) noexcept
{
	m_layout->Set(area);

	return true;
}

void UIComponent::ChangeOrigin(const Origin& origin) noexcept
{
	m_layout->Set(origin);
}

bool UIComponent::ChangePosition(int index, const Vector2& pos) noexcept
{
	if (index >= m_components.size()) return false;
	m_components[index].m_position = pos;

	return true;
}

void UIComponent::AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos)
{
	//auto prop = make_unique<TransformComponent>(move(comp), pos);
	auto component = TransformComponent(move(comp), pos);
	m_components.emplace_back(move(component));
}

void UIComponent::SetLayout(const UILayout& layout) noexcept
{
	if (m_layout == nullptr)
		m_layout = make_unique<UILayout>(layout);
	else
		*m_layout = layout;
}

UILayout* UIComponent::GetLayout() const noexcept
{
	return m_layout.get();
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
	operation.Process("Enable", m_enable);
	operation.Process("Components", m_components);
}

XMINT2 UIComponent::GetPositionByLayout(const XMINT2& position) noexcept
{
	return position + m_layout->GetPosition();
}

void UIComponent::SetEnable(bool enable)
{
	m_enable = enable;
}