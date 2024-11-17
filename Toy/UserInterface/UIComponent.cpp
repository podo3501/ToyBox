#include "pch.h"
#include "UIComponent.h"
#include "UILayout.h"
#include "UIType.h"
#include "JsonHelper.h"
#include "../Utility.h"
#include "TransformComponent.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_name{},
	m_layout{ make_unique<UILayout>(Rectangle{}, Origin::Center) }
{}

UIComponent& UIComponent::operator=(const UIComponent& other)
{
	if (this == &other) return *this;

	m_name = other.m_name;
	m_layout = std::make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& prop) {
		return make_unique<TransformComponent>(*prop.get());
		});

	return *this;
}

string UIComponent::GetType() const { return string(typeid(UIComponent).name()); }

unique_ptr<UIComponent> UIComponent::Clone() 
{
	return make_unique<UIComponent>(*this);
}

//template 함수로 만들지 않는 이유는 == 비교가 사방에서 쓰기 때문에 이 함수와 다른 함수가 충돌될지 모른다.
//namespace로 만들어주던지 아니면 cpp에 정의 해서 여기만 쓰도록 강제해야 한다.
bool operator==(const unique_ptr<TransformComponent>& lhs, const unique_ptr<TransformComponent>& rhs) {
	if (lhs == nullptr && rhs == nullptr) {
		return true;
	}
	if (lhs == nullptr || rhs == nullptr) {
		return false;
	}
	return *lhs == *rhs;
}

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetType() != o.GetType()) return false;
	return tie(m_name, *m_layout, m_components) == tie(o.m_name, *o.m_layout, o.m_components);
}

UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_layout = make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& prop) {
		return make_unique<TransformComponent>(*prop.get());
		});
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_components{ move(o.m_components) }
{}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& prop) {
		return prop->LoadResources(load);
		});
}

TransformComponent* UIComponent::FindTransformComponent(const string& name) const noexcept
{
	auto find = ranges::find_if(m_components, [&name](const auto& prop) {
		return prop->GetName() == name;
		});

	if (find == m_components.end()) return nullptr;
	return find->get();
}

void UIComponent::SetSelected(const string& name, bool selected) noexcept
{
	auto TransformComponent = FindTransformComponent(name);
	if (TransformComponent == nullptr) return;

	TransformComponent->SetSelected(selected);
}

UIComponent* UIComponent::GetSelected() const noexcept
{
	auto find = ranges::find_if(m_components, [](const auto& prop) {
		return prop->IsSelected();
		});

	if (find == m_components.end())
		return nullptr;

	return (*find)->GetComponent();
}

bool UIComponent::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
{
	return ranges::all_of(m_components, [&position, tracker](const auto& prop) {
		return prop->Update(position, tracker);
		});
}

void UIComponent::Render(IRender* render)
{
	ranges::for_each(m_components, [render](const auto& prop) {
		prop->Render(render);
		});
}

bool UIComponent::IsPicking(const Vector2& pos)  const noexcept
{
	if (m_layout->IsArea(pos)) return true;

	return ranges::any_of(m_components, [&pos](const auto& prop) {
		return prop->IsPicking(pos);
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

bool UIComponent::ChangeArea(const Rectangle& area) noexcept
{
	m_layout->Set(area);

	return true;
}

void UIComponent::SetName(const string& name) noexcept
{
	m_name = name;
}

const string& UIComponent::GetName() const noexcept
{
	return m_name;
}

void UIComponent::AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos)
{
	auto prop = make_unique<TransformComponent>(move(comp), pos);
	m_components.emplace_back(move(prop));
}

void UIComponent::SetLayout(const UILayout& layout) noexcept
{
	(*m_layout.get()) = layout;
}

UILayout* UIComponent::GetLayout() const noexcept
{
	return m_layout.get();
}

void to_json(nlohmann::ordered_json& j, const UIComponent& data)
{
	DataToJson("Name", data.m_name, j);
	DataToJson("Layout", data.m_layout, j);
	DataToJson("Components", data.m_components, j);
}

void from_json(const nlohmann::json& j, UIComponent& data)
{
	DataFromJson("Name", data.m_name, j);
	DataFromJson("Layout", data.m_layout, j);
	DataFromJson("Components", data.m_components, j);
}