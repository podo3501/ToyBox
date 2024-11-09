#include "pch.h"
#include "Dialog.h"
#include "../Include/IRenderer.h"
#include "Panel.h"
#include "UILayout.h"
#include "UIType.h"
#include "BGImage.h"
#include "../Utility.h"
#include "JsonHelper.h"

class RenderItemProperty
{
public:
	RenderItemProperty(unique_ptr<IRenderItem> renderItem, const Vector2& position) :
		m_renderItem{ move(renderItem) }, m_position{ position }, m_selected{ false }
	{}

	RenderItemProperty(const RenderItemProperty& other)
	{
		m_renderItem = other.m_renderItem->Clone();
		m_position = other.m_position;
		m_selected = false;
	}

	bool LoadResources(ILoadData* load)
	{
		return m_renderItem->LoadResources(load);
	}

	bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
	{
		return m_renderItem->Update(m_position + position, tracker);
	}

	void SetSelected(bool selected) noexcept
	{
		m_selected = selected;
	}

	bool IsSelected() const noexcept
	{
		return m_selected;
	}

	void Render(IRender* render)
	{
		m_renderItem->Render(render);
	}

	void SetPosition(const Vector2& position) noexcept
	{
		m_position = position;
	}

	IRenderItem* GetRenderItem() const noexcept
	{
		return m_renderItem.get();
	}

	const string& GetName() const
	{
		return m_renderItem->GetName();
	}

private:
	unique_ptr<IRenderItem> m_renderItem;
	Vector2 m_position;
	bool m_selected;
};

class RenderItemList
{
public:
	RenderItemList() = default;
	~RenderItemList() = default;
	RenderItemList(const RenderItemList& other)
	{
		ranges::transform(other.m_properties, back_inserter(m_properties), [](const auto& prop) {
			return make_unique<RenderItemProperty>(*prop.get());
			});
	}

	bool LoadResources(ILoadData* load)
	{
		return ranges::all_of(m_properties, [load](const auto& prop) {
			return prop->LoadResources(load);
			});
	}

	void SetSelected(const string& name, bool selected) noexcept
	{
		auto property = FindProperty(name);
		if (property == nullptr) return;

		property->SetSelected(selected);
	}

	IRenderItem* GetSelected() const noexcept
	{
		auto find = ranges::find_if(m_properties, [](const auto& prop) {
			return prop->IsSelected();
			});

		if (find == m_properties.end())
			return nullptr;

		return (*find)->GetRenderItem();
	}

	bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
	{
		return ranges::all_of(m_properties, [&position, tracker](const auto& prop) {
			return prop->Update(position, tracker);
			});
	}

	void Render(IRender* render)
	{
		ranges::for_each(m_properties, [render](const auto& prop) {
			prop->Render(render);
			});
	}

	void AddComponent(unique_ptr<IRenderItem> item, const Vector2& position)
	{
		auto prop = make_unique<RenderItemProperty>(move(item), position);
		m_properties.emplace_back(move(prop));
	}

	void SetPosition(const string& name, const Vector2& position) noexcept
	{
		auto property = FindProperty(name);
		if (property == nullptr) return;

		property->SetPosition(position);
	}

	IRenderItem* GetRenderItem(const string& name) const noexcept
	{
		auto property = FindProperty(name);
		if (property == nullptr) return nullptr;

		return property->GetRenderItem();
	}

private:
	RenderItemProperty* FindProperty(const string& name) const noexcept
	{
		auto find = ranges::find_if(m_properties, [&name](const auto& prop) {
			return prop->GetName() == name;
			});

		if (find == m_properties.end()) return nullptr;
		return find->get();
	}

	vector<unique_ptr<RenderItemProperty>> m_properties;
};

using json = nlohmann::json;

Dialog::Dialog() :
	m_layout{ make_unique<UILayout>(Rectangle{}, Origin::Center) },
	m_renderItemList{ make_unique<RenderItemList>() }
{};
Dialog::~Dialog() = default;

bool Dialog::LoadResources(ILoadData* load)
{
	return m_renderItemList->LoadResources(load);
}

void Dialog::SetSelected(const string& name, bool selected) noexcept
{
	return m_renderItemList->SetSelected(name, selected);
}

IRenderItem* Dialog::GetSelected() const noexcept
{
	return m_renderItemList->GetSelected();
}

bool Dialog::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
{
	return m_renderItemList->Update(position, tracker);
}

void Dialog::Render(IRender* render)
{
	m_renderItemList->Render(render);
}

bool Dialog::IsPicking(const Vector2& pos)  const noexcept
{
	return m_layout->IsArea(pos);
}

const Rectangle& Dialog::GetArea() const noexcept
{
	return m_layout->GetArea();
}

bool Dialog::SetResources(const wstring& filename)
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
		case DataType::Component:
			auto [item, position] = GetComponent(data);
			ReturnIfNullptr(item);

			m_layout->Union(item->GetArea());	//자식의 크기만큼 자신의 크기를 키운다.
			m_renderItemList->AddComponent(move(item), position);
			break;
		}
	}
	
	return true;
}

void Dialog::SetName(const string& name)
{
	m_name = name;
}

void Dialog::AddComponent(unique_ptr<IRenderItem>&& item, const Vector2& pos)
{
	m_renderItemList->AddComponent(move(item), pos);
}

Dialog::Dialog(const Dialog& other) noexcept
{
	m_name = other.m_name + "_clone";
	m_layout = make_unique<UILayout>(*other.m_layout);
	m_renderItemList = make_unique<RenderItemList>(*other.m_renderItemList);
}

unique_ptr<IRenderItem> Dialog::Clone()
{ 
	return make_unique<Dialog>(*this);
}

void Dialog::SetPosition(const string& name, const Vector2& position) noexcept
{
	m_renderItemList->SetPosition(name, position);
}

IRenderItem* Dialog::GetRenderItem(const string& name) const noexcept
{
	return m_renderItemList->GetRenderItem(name);
}