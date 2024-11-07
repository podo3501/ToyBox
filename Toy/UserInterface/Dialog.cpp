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
	m_layout{ make_unique<UILayout>(Rectangle{}, Vector2{}, Origin::Center) },
	m_panel{ make_unique<Panel>() }
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
	return m_panel->IsPicking(pos);
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

		auto [item, position] = GetComponent(data);
		ReturnIfNullptr(item);
		
		m_layout->Union(item->GetArea());	//자식의 크기만큼 자신의 크기를 키운다.
		auto rmProp = make_unique<RenderItemProperty>(move(item), position, true);
		m_renderItems.emplace_back(move(rmProp));
	}
	
	return true;
}

bool Dialog::SetUIItem()
{
	//UILayout layout({ 0, 0, 220, 190 }, { 0.0f, 0.0f }, Origin::LeftTop);
	//ImageSource bgImgSrc{
	//	L"UI/Blue/button_square_header_large_square_screws.png", {
	//		{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
	//		{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
	//		{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
	//	}
	//};
	//unique_ptr<BGImage> bgImg = make_unique<BGImage>();
	//bgImg->SetImage(L"Resources/", m_renderer, bgImgSrc, layout);

	////unique_ptr<IRenderItem> = uiLoad->Load(abc.json);
	//m_panel->AddRenderItem({ 0.1f, 0.1f }, move(bgImg));

	return true;
}
