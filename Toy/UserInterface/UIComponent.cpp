#include "pch.h"
#include "UIComponent.h"
#include "UILayout.h"
#include "UIType.h"
#include "JsonHelper.h"
#include "JsonOperation.h"
#include "../Utility.h"
#include "Property.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

class TestClass
{
public:
	TestClass()
	{
		//int a = 1;
	}

	TestClass(const TestClass& /*other*/)
	{
		//int a = 1;
	}

	friend void to_json(nlohmann::ordered_json& j, const TestClass& tc);
	friend void from_json(const nlohmann::json& j, TestClass& tc);

private:
	int a{ 5 };
	int b{ 6 };
};

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_name{},
	m_layout{ make_unique<UILayout>(Rectangle{}, Origin::Center) }
{
	m_testClass.emplace_back(make_unique<TestClass>());
	m_testClass.emplace_back(make_unique<TestClass>());
}

UIComponent& UIComponent::operator=(const UIComponent& other)
{
	if (this == &other) return *this;

	m_name = other.m_name;
	m_layout = std::make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_properties, back_inserter(m_properties), [](const auto& prop) {
		return make_unique<Property>(*prop.get());
		});

	return *this;
}

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

Property* UIComponent::FindProperty(const string& name) const noexcept
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

void UIComponent::FileIO(JsonOperation* operation) noexcept
{
	operation->Push(m_name);
	m_layout->FileIO(operation);	
	//Process(operation);
	//m_testClass
	operation->Pop();
}

//unique_ptr<UIComponent> UIComponent::CreateComponent(JsonOperation* operation)
//{
//	operation->FindComponent();
//}

void to_json(nlohmann::ordered_json& j, const UIComponent& data)
{
	DataToJson("Name", data.m_name, j);
	DataToJson("Layout", data.m_layout, j);
	DataToJson("Properties", data.m_properties, j);

	j["TestClass"] = json::array();
	for (const auto& testClassPtr : data.m_testClass)
	{
		if (!testClassPtr) continue;
		j["TestClass"].push_back(*testClassPtr);
	}
}

void from_json(const nlohmann::json& j, UIComponent& data)
{
	DataFromJson("Name", data.m_name, j);
	DataFromJson("Layout", data.m_layout, j);
	DataFromJson("Properties", data.m_properties, j);

	//data.m_testClass.clear();
	//// JSON 배열을 순회하면서 unique_ptr<Property>로 변환
	//for (const auto& item : j.at("TestClass")) 
	//{
	//	auto testClass = std::make_unique<TestClass>(item.get<TestClass>());
	//	data.m_testClass.push_back(std::move(testClass));
	//}
}

void to_json(nlohmann::ordered_json& j, const TestClass& tc)
{
	j = nlohmann::json{ {"a", tc.a}, {"b", tc.b} };
}

void from_json(const nlohmann::json& j, TestClass& tc)
{
	j.at("a").get_to(tc.a);
	j.at("b").get_to(tc.b);
}