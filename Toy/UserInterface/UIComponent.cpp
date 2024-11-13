#include "pch.h"
#include "UIComponent.h"
#include "UILayout.h"
#include "UIType.h"
#include "JsonHelper.h"
#include "../Utility.h"

class UIComponent::Property
{
public:
	Property(unique_ptr<UIComponent> comp, const Vector2& position) :
		m_component{ move(comp) }, m_position{ position }, m_selected{ false }
	{}

	Property(const Property& other)
	{
		m_component = other.m_component->Clone();
		m_position = other.m_position;
		m_selected = false;
	}

	inline bool LoadResources(ILoadData* load) { return m_component->LoadResources(load); }
	inline bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept { return m_component->Update(m_position + position, tracker); }
	inline void SetSelected(bool selected) noexcept { m_selected = selected; }
	inline bool IsSelected() const noexcept { return m_selected; }
	inline bool IsPicking(const Vector2& pos) const noexcept { return m_component->IsPicking(pos); }
	inline void Render(IRender* render) { m_component->Render(render); }
	inline void SetPosition(const Vector2& position) noexcept { m_position = position; }
	inline UIComponent* GetComponent() const noexcept { return m_component.get(); }
	inline const string& GetName() const { return m_component->GetName(); }

private:
	unique_ptr<UIComponent> m_component;
	Vector2 m_position;
	bool m_selected;
};

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_layout{ make_unique<UILayout>(Rectangle{}, Origin::Center) }
{}

UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name + "_clone";
	m_layout = make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_properties, back_inserter(m_properties), [](const auto& prop) {
		return make_unique<Property>(*prop.get());
		});
}

bool UIComponent::IsEqual(const UIComponent* other) const noexcept
{
	//_clone 같은게 있을 수도 있고, 다른 이름이지만 값은 같을 수도 있기 때문에 m_name은 비교하지 않는다.
	if (!m_layout->IsEqual(other->m_layout.get())) return false;
	//Property 비교

	return true;
}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_properties, [load](const auto& prop) {
		return prop->LoadResources(load);
		});
}

UIComponent::Property* UIComponent::FindProperty(const string& name) const noexcept
{
	auto find = ranges::find_if(m_properties, [&name](const auto& prop) {
		return prop->GetName() == name;
		});

	if (find == m_properties.end()) return nullptr;
	return find->get();
}

void UIComponent::SetSelected(const string& name, bool selected) noexcept
{
	auto property = FindProperty(name);
	if (property == nullptr) return;

	property->SetSelected(selected);
}

UIComponent* UIComponent::GetSelected() const noexcept
{
	auto find = ranges::find_if(m_properties, [](const auto& prop) {
		return prop->IsSelected();
		});

	if (find == m_properties.end())
		return nullptr;

	return (*find)->GetComponent();
}

bool UIComponent::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
{
	return ranges::all_of(m_properties, [&position, tracker](const auto& prop) {
		return prop->Update(position, tracker);
		});
}

void UIComponent::Render(IRender* render)
{
	ranges::for_each(m_properties, [render](const auto& prop) {
		prop->Render(render);
		});
}

bool UIComponent::IsPicking(const Vector2& pos)  const noexcept
{
	if (m_layout->IsArea(pos)) return true;

	return ranges::any_of(m_properties, [&pos](const auto& prop) {
		return prop->IsPicking(pos);
		});
}

const Rectangle& UIComponent::GetArea() const noexcept
{
	return m_layout->GetArea();
}

bool UIComponent::SetResources(const wstring& filename)
{
	const json& dataList = LoadUIFile(filename);
	if (dataList.is_null())
		return false;

	for (const auto& [key, data] : dataList.items())
	{
		auto dataType = GetType(key);
		if (dataType == DataType::Init) return false;

		switch (dataType)
		{
		case DataType::Name:
			m_name = dataList["Name"];
			break;
		case DataType::Layout:
			m_layout = move(make_unique<UILayout>(data));
			break;
		case DataType::Component:
		{
			auto [comp, position] = CreateComponent(data);
			ReturnIfNullptr(comp);

			m_layout->Union(comp->GetArea());	//자식의 크기만큼 자신의 크기를 키운다.
			AddComponent(move(comp), position);
			break;
		}
		case DataType::Property:
			ReturnIfFalse(ReadProperty(data));
			break;
		}
	}

	return true;
}

void UIComponent::SetChildPosition(const string& name, const Vector2& position) noexcept
{
	auto property = FindProperty(name);
	if (property == nullptr) return;

	property->SetPosition(position);
}

UIComponent* UIComponent::GetComponent(const string& name) const noexcept
{
	auto property = FindProperty(name);
	if (property == nullptr) return nullptr;

	return property->GetComponent();
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
	auto prop = make_unique<Property>(move(comp), pos);
	m_properties.emplace_back(move(prop));
}

void UIComponent::SetLayout(const UILayout& layout) noexcept
{
	(*m_layout.get()) = layout;
}

UILayout* UIComponent::GetLayout() const noexcept
{
	return m_layout.get();
}

void UIComponent::ToJson(ordered_json& outJson) const noexcept
{
	DataToJson("Name", m_name, outJson);
	ordered_json j;
	m_layout->ToJson(j);
	outJson["Layout"] = j;
}

void UIComponent::FromJson(const json& j) noexcept
{
	DataFromJson("Name", m_name, j);
	m_layout->FromJson(j["Layout"]);
}