#include "pch.h"
#include "Popup.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "Utility.h"
#include "../Toy/HelperClass.h"

using namespace Tool;

Popup::Popup(IRenderer* renderer) :
	m_renderer{ renderer },
	m_scene{ make_unique<Scene>(GetRectResolution()) }
{
	m_renderer->AddLoadScene(m_scene.get());
}

Popup::~Popup()
{}

bool Popup::Excute(MouseTracker* mouseTracker)
{
	m_position = XMUINT2ToImVec2(mouseTracker->GetOffsetPosition());

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
	ImVec2 size = XMUINT2ToImVec2(m_scene->GetSize());
	// 텍스처를 사각형 형태로 그리기
	ImVec2 mousePos = ImGui::GetMousePos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddImage(
		m_textureID,
		m_position,                                // 시작 좌표
		ImVec2(m_position.x + size.x, m_position.y + size.y) // 종료 좌표
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

bool Popup::CreateScene(const XMUINT2& size)
{
	m_scene->SetSize(size);
	ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_scene.get(), m_textureID));
	m_draw = true;

	return true;
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

	CreateScene({ 170, 120 });
	m_scene->AddComponent({ 0.f, 0.f }, move(bgImg));

	ReturnIfFalse(m_renderer->LoadScenes());
	ReturnIfFalse(m_scene->SetDatas(m_renderer->GetValue()));
	ReturnIfFalse(m_scene->Update(nullptr));	//position 갱신. 안하면 {0, 0}에 1프레임 정도 잠깐 나타난다.
	
	return true;
}