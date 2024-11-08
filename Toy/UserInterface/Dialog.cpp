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
	RenderItemProperty(unique_ptr<IRenderItem> renderItem, const Vector2& position, bool selected) :
		m_renderItem{ move(renderItem) }, m_position{ position }, m_selected{ selected }
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

	bool Update(const Vector2& position) noexcept
	{
		return m_renderItem->Update(m_position + position );
	}

	bool IsSelected() const noexcept
	{
		return m_selected;
	}

	void Render(IRender* render)
	{
		m_renderItem->Render(render);
	}

	IRenderItem* GetRenderItem() const noexcept
	{
		return m_renderItem.get();
	}

private:
	unique_ptr<IRenderItem> m_renderItem;
	Vector2 m_position;
	bool m_selected;
};

using json = nlohmann::json;

Dialog::Dialog() :
	m_layout{ make_unique<UILayout>(Rectangle{}, Vector2{}, Origin::Center) }
{};
Dialog::~Dialog() = default;

bool Dialog::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_renderItems, [load](const auto& item) {
		return item->LoadResources(load);
		});
}

IRenderItem* Dialog::GetSelected() const noexcept
{
	auto findItem = ranges::find_if(m_renderItems, [](const auto& item) {
		return item->IsSelected();
		});

	if (findItem == m_renderItems.end())
		return nullptr;

	return (*findItem)->GetRenderItem();
}

bool Dialog::Update(const Vector2& position) noexcept
{
	return ranges::all_of(m_renderItems, [&position](const auto& item) {
		return item->Update(position);
		});
}

void Dialog::Render(IRender* render)
{
	ranges::for_each(m_renderItems, [render](const auto& item) {
		item->Render(render);
		});
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
			auto rmProp = make_unique<RenderItemProperty>(move(item), position, true);
			m_renderItems.emplace_back(move(rmProp));
			break;
		}
	}
	
	return true;
}

void Dialog::SetName(const string& name)
{
	m_name = name;
}

void Dialog::AddRenderItem(const Vector2& pos, unique_ptr<IRenderItem>&& renderItem)
{
	auto riProp = make_unique<RenderItemProperty>(move(renderItem), pos, false);
	m_renderItems.emplace_back(move(riProp));
}

Dialog::Dialog(const Dialog& other) noexcept
{
	m_name = other.m_name + "_clone";
	m_layout = make_unique<UILayout>(*other.m_layout);
	ranges::transform(other.m_renderItems, back_inserter(m_renderItems), [](const auto& item) {
		return make_unique<RenderItemProperty>(*item.get());
		});
}

unique_ptr<IRenderItem> Dialog::Clone()
{ 
	return make_unique<Dialog>(*this);
}

void Dialog::SetPosition(const string& name, const Vector2& position) noexcept
{
	position;
}