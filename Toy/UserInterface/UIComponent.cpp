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

UIComponent& UIComponent::operator=(const UIComponent& other)
{
	if (this == &other) return *this;

	m_name = other.m_name;
	m_layout = std::make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& transComp) {
		return make_unique<TransformComponent>(*transComp.get());
		});

	return *this;
}

string UIComponent::GetType() const { return string(typeid(*this).name()); }

unique_ptr<UIComponent> UIComponent::Clone() 
{
	return make_unique<UIComponent>(*this);
}

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetType() != o.GetType()) return false;

	ReturnIfFalse(tie(m_name) == tie(o.m_name));
	ReturnIfFalse(CompareUniquePtr(m_layout, o.m_layout));
	ReturnIfFalse(CompareSeq(m_components, o.m_components));

	return true;
}

UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_layout = make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& transComp) {
		return make_unique<TransformComponent>(*transComp.get());
		});
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_components{ move(o.m_components) }
{}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& transComp) {
		return transComp->LoadResources(load);
		});
}

TransformComponent* UIComponent::FindTransformComponent(const string& name) const noexcept
{
	auto find = ranges::find_if(m_components, [&name](const auto& transComp) {
		return transComp->GetName() == name;
		});

	if (find == m_components.end()) return nullptr;
	return find->get();
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
	return ranges::all_of(m_components, [value](const auto& prop) {
		return prop->SetDatas(value);
		});
}

bool UIComponent::ProcessUpdate(const XMINT2& position, InputManager* inputManager) noexcept
{
	if (!m_enable) return true;

	Update(position, inputManager);

	return ranges::all_of(m_components, [this, &position, inputManager](const auto& transComp) {
		const auto& curPosition = m_layout->GetPosition(transComp->GetPosition()) + position;
		if (inputManager)
			inputManager->GetMouse()->SetOffset(curPosition);
		return transComp->ProcessUpdate(curPosition, inputManager);
		});
}

void UIComponent::ProcessRender(IRender* render)
{
	if (!m_enable) 
		return;

	Render(render);

	ranges::for_each(m_components, [render](const auto& transComp) {
		transComp->ProcessRender(render);
		});
}

const Rectangle& UIComponent::GetArea() const noexcept
{
	return m_layout->GetArea();
}

void UIComponent::SetChildPosition(const string& name, const Vector2& position) noexcept
{
	auto TransformComponent = FindTransformComponent(name);
	if (TransformComponent == nullptr) return;

	TransformComponent->SetPosition(position);
}

UIComponent* UIComponent::GetComponent(const string& name) const noexcept
{
	auto TransformComponent = FindTransformComponent(name);
	if (TransformComponent == nullptr) return nullptr;

	return TransformComponent->GetComponent();
}

vector<UIComponent*> UIComponent::GetComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_components, back_inserter(componentList), [](const auto& transCompo) {
		return transCompo->GetComponent();
		});

	return componentList;
}

bool UIComponent::IsHover(const XMINT2& pos) const noexcept
{
	if (m_layout->IsArea(pos)) return true;

	return ranges::any_of(m_components, [this, &pos](const auto& transComponent) {
		const auto& curPosition = pos - m_layout->GetPosition(transComponent->GetPosition());
		return transComponent->IsHover(curPosition);
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

	ranges::for_each(m_components, [this, &pos, &outList](auto& transComponent) {
		const auto& curPosition = pos - m_layout->GetPosition(transComponent->GetPosition());
		transComponent->GetComponents(curPosition, outList);
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
	m_components[index]->SetPosition(pos);

	return true;
}

void UIComponent::AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos)
{
	auto prop = make_unique<TransformComponent>(move(comp), pos);
	m_components.emplace_back(move(prop));
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