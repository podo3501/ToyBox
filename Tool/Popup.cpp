#include "pch.h"
#include "Popup.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "Utility.h"
#include "../Toy/HelperClass.h"

using namespace Tool;

Popup::Popup(IRenderer* renderer) :
	m_renderer{ renderer }
{}

Popup::~Popup()
{
	Reset();
}

void Popup::Reset() noexcept
{
	m_draw = false;
	m_currentAction.reset();
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveComponent(m_component.get());
	m_textureID = 0;
}

bool Popup::IsComponent() const noexcept
{
	return m_component != nullptr;
}

unique_ptr<UIComponent> Popup::GetComponent() noexcept
{
	Reset();
	return move(m_component);
}

bool Popup::Excute(MouseTracker* mouseTracker)
{
	if (!m_currentAction.has_value()) return true;

	auto result{ true };
	switch (m_currentAction.value())
	{
	case MakeComponent::Dialog: result = MakeDialog(); break;
	}

	m_currentAction.reset(); // 상태 초기화

	return true;
}

void Popup::DrawMakeComponent()
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

void Popup::Show()
{
	if (m_draw)
	{
		DrawMakeComponent();
		return;
	}

	// 마우스 오른쪽 버튼 클릭 시 팝업 메뉴 띄우기
	if (!ImGui::BeginPopupContextWindow("PopupMenu")) return;
	
	if (ImGui::MenuItem("Dialog")) m_currentAction = MakeComponent::Dialog;
	if (ImGui::MenuItem("Option 2")) {}
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool Popup::MakeDialog()
{
	UILayout layout({ 0, 0, 170, 120 }, Origin::LeftTop);
	ImageSource dialogSource{
		L"UI/Blue/button_square_header_large_square_screws.png", {
			{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
			{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
			{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
		}
	};
	unique_ptr<BGImage> bgImg = make_unique<BGImage>();
	bgImg->SetImage("untitled_dialog", layout, dialogSource);
	m_component = move(bgImg);

	ReturnIfFalse(m_renderer->CreateRenderTexture(m_component->GetSize(), m_component.get(), m_textureID));
	m_renderer->AddComponent(m_component.get(), false);
	m_renderer->LoadComponents();
	m_draw = true;

	return true;
}