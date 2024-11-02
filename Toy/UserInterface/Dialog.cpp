#include "pch.h"
#include "Dialog.h"
#include "../Include/IRenderer.h"
#include "Panel.h"
#include "UILayout.h"
#include "UIType.h"
#include "BGImage.h"
#include "../Utility.h"
#include "JsonHelper.h"

using json = nlohmann::json;

Dialog::Dialog() :
	m_panel{ make_unique<Panel>() }
{};
Dialog::~Dialog() = default;

bool Dialog::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_renderItems, [load](const auto& item) {
		return item.second->LoadResources(load);
		});
}

void Dialog::Render(IRender* render)
{
	render;
}

bool Dialog::IsPicking(const Vector2& pos)  const noexcept
{
	return m_panel->IsPicking(pos);
}

const Rectangle& Dialog::GetArea() const noexcept
{
	return m_panel->GetArea();
}

bool Dialog::SetResources(const wstring& filename)
{ 
	ifstream file(filename);
	ReturnIfFalse(file.is_open());

	json dataList = json::parse(file);
	for (const auto& [component, data] : dataList.items())
	{
		auto [item, position] = GetComponent(component, data);
		ReturnIfNullptr(item);

		m_renderItems.emplace_back(make_pair(position, move(item)));
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
