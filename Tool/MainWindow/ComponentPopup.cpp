#include "pch.h"
#include "ComponentPopup.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/UIUtility.h"

ComponentPopup::ComponentPopup(IRenderer* renderer, const string& mainWndName) noexcept :
	m_renderer{ renderer },
	m_name{ "PopupMenu_" + mainWndName }
{}

ComponentPopup::~ComponentPopup()
{
	Reset();
}

void ComponentPopup::Reset() noexcept
{
	m_draw = false;
	m_currentAction.reset();
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveRenderComponent(m_component.get());
	m_textureID = 0;
}

bool ComponentPopup::IsComponent() const noexcept
{
	return m_component != nullptr;
}

unique_ptr<UIComponent> ComponentPopup::GetComponent() noexcept
{
	Reset();
	return move(m_component);
}

bool ComponentPopup::Excute()
{
	if (!m_currentAction.has_value()) return true;

	auto result{ true };
	switch (m_currentAction.value())
	{
	case MakeComponent::ImageGrid1: result = MakeImageGrid1(); break;
	case MakeComponent::ImageGrid3: result = MakeImageGrid3(); break;
	case MakeComponent::ImageGrid9: result = MakeImageGrid9(); break;
	}

	m_currentAction.reset(); // 상태 초기화

	return true;
}

void ComponentPopup::DrawMakeComponent()
{
	const auto size = XMUINT2ToImVec2(m_component->GetSize());
	ImVec2 halfSize{ size.x / 2.f, size.y / 2.f };
	// 텍스처를 사각형 형태로 그리기
	m_position = ImGui::GetMousePos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	ImU32 colorU32 = ImGui::GetColorU32(tintColor);

	drawList->AddImage(
		m_textureID,
		{ m_position.x, m_position.y },                      // 시작 좌표
		{ m_position.x + size.x, m_position.y + size.y },    // 종료 좌표
		ImVec2(0, 0),
		ImVec2(1, 1),
		colorU32
	);
}

void ComponentPopup::Render()
{
	if (m_draw)
	{
		DrawMakeComponent();
		return;
	}

	// 마우스 오른쪽 버튼 클릭 시 팝업 메뉴 띄우기
	if (!ImGui::BeginPopupContextWindow(m_name.c_str()))
	{
		m_isActive = false;
		return;
	}
	
	m_isActive = true;
	if (ImGui::MenuItem("Image Grid 1")) m_currentAction = MakeComponent::ImageGrid1;
	if (ImGui::MenuItem("Image Grid 3")) m_currentAction = MakeComponent::ImageGrid3;
	if (ImGui::MenuItem("Image Grid 9")) m_currentAction = MakeComponent::ImageGrid9;
	//if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool ComponentPopup::IsActive() const noexcept
{
	return m_isActive;
}

bool ComponentPopup::LoadImageGrid(unique_ptr<UIComponent>&& imgGrid)
{
	ReturnIfNullptr(imgGrid);

	m_component = move(imgGrid);
	ReturnIfFalse(m_renderer->CreateRenderTexture(m_component->GetSize(), m_component.get(), m_textureID));

	ReturnIfFalse(m_renderer->LoadComponent(m_component.get()));
	m_draw = true;

	return true;
}

//검게 나오는 문제는 좌표 문제이다.
bool ComponentPopup::LoadComponent(unique_ptr<UIComponent>&& imgGrid)
{
	ReturnIfNullptr(imgGrid);

	m_component = move(imgGrid);
	ReturnIfFalse(m_renderer->CreateRenderTexture(m_component->GetSize(), m_component.get(), m_textureID));
	m_draw = true;
	
	return true;
}


bool ComponentPopup::MakeImageGrid1()
{
	UILayout layout({ 64, 64 }, Origin::LeftTop);
	ImageSource source{ L"UI/SampleTexture/ToolComponentPopup.png", { { 2, 52, 64, 64 } } };
	return LoadImageGrid(CreateImageGrid<ImageGrid1>("untitled_imageGrid1", layout, source));
}

bool ComponentPopup::MakeImageGrid3()
{
	UILayout layout({ 48, 48 }, Origin::LeftTop);
	ImageSource source{
		L"UI/SampleTexture/ToolComponentPopup.png", {
			{ 2, 2, 23, 48 }, { 25, 2, 2, 48 }, { 27, 2, 23, 48 }
		}
	};
	return LoadImageGrid(CreateImageGrid<ImageGrid3>("untitled_imageGrid3", layout, source));
}

bool ComponentPopup::MakeImageGrid9()
{
	UILayout layout({ 170, 120 }, Origin::LeftTop);
	ImageSource source{
		L"UI/SampleTexture/ToolComponentPopup.png", {
			{ 2, 52, 30, 36 }, { 32, 52, 4, 36 }, { 36, 52, 30, 36 },
			{ 2, 88, 30, 2 }, { 32, 88, 4, 2 }, { 36, 88, 30, 2 },
			{ 2, 90, 30, 26 }, { 32, 90, 4, 26 }, { 36, 90, 30, 26 }
		}
	};
	return LoadImageGrid(CreateImageGrid<ImageGrid9>("untitled_imageGrid9", layout, source));
}
