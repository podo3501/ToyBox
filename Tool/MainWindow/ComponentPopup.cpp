#include "pch.h"
#include "ComponentPopup.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/BGImage.h"
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
	m_renderer->RemoveComponent(m_component.get());
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
	case MakeComponent::BGImage: result = MakeBGImage(); break;
	}

	m_currentAction.reset(); // ���� �ʱ�ȭ

	return true;
}

void ComponentPopup::DrawMakeComponent()
{
	const auto size = XMUINT2ToImVec2(m_component->GetSize());
	ImVec2 halfSize{ size.x / 2.f, size.y / 2.f };
	// �ؽ�ó�� �簢�� ���·� �׸���
	m_position = ImGui::GetMousePos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddImage(
		m_textureID,
		{ m_position.x, m_position.y }, // ���� ��ǥ
		{ m_position.x + size.x, m_position.y + size.y } // ���� ��ǥ
	);
}

void ComponentPopup::Show()
{
	if (m_draw)
	{
		DrawMakeComponent();
		return;
	}

	// ���콺 ������ ��ư Ŭ�� �� �˾� �޴� ����
	if (!ImGui::BeginPopupContextWindow(PopupName))
		return;
	
	if (ImGui::MenuItem("Background Image")) m_currentAction = MakeComponent::BGImage;
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool ComponentPopup::IsShowed() const noexcept
{
	return ImGui::IsPopupOpen(PopupName);
}

bool ComponentPopup::MakeBGImage()
{
	UILayout layout({ 0, 0, 170, 120 }, Origin::LeftTop);
	ImageSource source{
		L"UI/Blue/button_square_header_large_square_screws.png", {
			{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
			{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
			{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
		}
	};
	unique_ptr<BGImage> bgImg = make_unique<BGImage>();
	bgImg->SetImage("untitled_bgImage", layout, source);
	m_component = move(bgImg);

	ReturnIfFalse(m_renderer->CreateRenderTexture(m_component->GetSize(), m_component.get(), m_textureID));
	m_renderer->AddComponent(m_component.get(), false);
	m_renderer->LoadComponents();
	m_draw = true;

	return true;
}