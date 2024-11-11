#include "pch.h"
#include "IRenderItem.h"
#include "UILayout.h"
#include "UIType.h"
#include "JsonHelper.h"
#include "../Utility.h"

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

	bool IsPicking(const Vector2& pos) const noexcept
	{
		return m_renderItem->IsPicking(pos);
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

class IRenderItem::Implementation
{
public:
	Implementation() :
		m_layout{ make_unique<UILayout>(Rectangle{}, Origin::Center) }
	{}

	~Implementation() = default;
	Implementation(const Implementation& other)
	{
		m_name = other.m_name + "_clone";
		m_layout = make_unique<UILayout>(*other.m_layout);
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

	bool IsPicking(const Vector2& pos)  const noexcept
	{
		if(m_layout->IsArea(pos)) return true;

		return ranges::any_of(m_properties, [&pos](const auto& prop) {
			return prop->IsPicking(pos);
			});
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

	void SetLayout(unique_ptr<UILayout> layout) noexcept
	{
		m_layout = move(layout);
	}

	void SetLayout(const UILayout& layout) noexcept
	{
		(*m_layout.get()) = layout;
	}

	UILayout* GetLayout() const noexcept
	{
		return m_layout.get();
	}

	void SetName(const string& name) noexcept
	{
		m_name = name;
	}

	const string& GetName() const noexcept
	{
		return m_name;
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

	string m_name{};
	unique_ptr<UILayout> m_layout;
	vector<unique_ptr<RenderItemProperty>> m_properties;
};

using json = nlohmann::json;

IRenderItem::~IRenderItem() = default;
IRenderItem::IRenderItem() :
	m_impl{ make_unique<Implementation>() }
{}

IRenderItem::IRenderItem(const IRenderItem& other)
{
	m_impl = make_unique<Implementation>(*other.m_impl);
}

bool IRenderItem::LoadResources(ILoadData* load)
{
	return m_impl->LoadResources(load);
}

void IRenderItem::SetSelected(const string& name, bool selected) noexcept
{
	return m_impl->SetSelected(name, selected);
}

IRenderItem* IRenderItem::GetSelected() const noexcept
{
	return m_impl->GetSelected();
}

bool IRenderItem::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
{
	return m_impl->Update(position, tracker);
}

void IRenderItem::Render(IRender* render)
{
	m_impl->Render(render);
}

bool IRenderItem::IsPicking(const Vector2& pos)  const noexcept
{
	return m_impl->IsPicking(pos);
}

const Rectangle& IRenderItem::GetArea() const noexcept
{
	auto layout = m_impl->GetLayout();
	return layout->GetArea();
}

bool IRenderItem::SetResources(const wstring& filename)
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
			m_impl->SetName(dataList["Name"]);
			break;
		case DataType::Layout:
			m_impl->SetLayout(move(make_unique<UILayout>(data)));
			break;
		case DataType::Component:
		{
			auto [item, position] = GetComponent(data);
			ReturnIfNullptr(item);

			auto layout = m_impl->GetLayout();
			layout->Union(item->GetArea());	//자식의 크기만큼 자신의 크기를 키운다.
			m_impl->AddComponent(move(item), position);
			break;
		}
		case DataType::Property:
			ReturnIfFalse(ReadProperty(data));
			break;
		}
	}

	return true;
}

void IRenderItem::SetChildPosition(const string& name, const Vector2& position) noexcept
{
	m_impl->SetPosition(name, position);
}

IRenderItem* IRenderItem::GetRenderItem(const string& name) const noexcept
{
	return m_impl->GetRenderItem(name);
}

bool IRenderItem::ChangeArea(const Rectangle& area) noexcept
{
	auto layout = m_impl->GetLayout();
	layout->Set(area);

	return true;
}

void IRenderItem::SetName(const string& name) noexcept
{
	return m_impl->SetName(name);
}

const string& IRenderItem::GetName() const noexcept
{
	return m_impl->GetName();
}

void IRenderItem::AddComponent(unique_ptr<IRenderItem>&& item, const Vector2& pos)
{
	m_impl->AddComponent(move(item), pos);
}

void IRenderItem::SetLayout(const UILayout& layout) noexcept
{
	m_impl->SetLayout(layout);
}

UILayout* IRenderItem::GetLayout() const noexcept
{
	return m_impl->GetLayout();
}