#include "pch.h"
#include "ComponentPopup.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../Toy/HelperClass.h"

static constexpr const char* PopupName = "PopupMenu";

ComponentPopup::ComponentPopup(IRenderer* renderer) :
	m_renderer{ renderer }
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

bool ComponentPopup::Excute(MouseTracker* mouseTracker)
{
	if (!m_currentAction.has_value()) return true;

	auto result{ true };
	switch (m_currentAction.value())
	{
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
	drawList->AddImage(
		m_textureID,
		{ m_position.x, m_position.y }, // 시작 좌표
		{ m_position.x + size.x, m_position.y + size.y } // 종료 좌표
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
	if (!ImGui::BeginPopupContextWindow(PopupName))
	{
		m_isActive = false;
		return;
	}
	
	m_isActive = true;
	if (ImGui::MenuItem("Image Grid 9")) m_currentAction = MakeComponent::ImageGrid9;
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool ComponentPopup::IsActive() const noexcept
{
	return m_isActive;
}

bool ComponentPopup::MakeImageGrid9()
{
	UILayout layout({ 170, 120 }, Origin::LeftTop);
	ImageSource source{
		L"UI/Blue/button_square_header_large_square_screws.png", {
			{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
			{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
			{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
		}
	};
	unique_ptr<ImageGrid9> imgGrid9 = make_unique<ImageGrid9>();
	imgGrid9->SetImage("untitled_imageGrid9", layout, source);
	m_component = move(imgGrid9);

	ReturnIfFalse(m_renderer->CreateRenderTexture(m_component->GetSize(), m_component.get(), m_textureID));
	m_renderer->LoadComponent(m_component.get());
	m_draw = true;

	return true;
}
