#include "pch.h"
#include "FloatingComponent.h"
#include "IRenderer.h"
#include "UserInterface/UIComponent/Components/RenderTexture.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/UIComponent/Components/TextArea.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "UserInterface/UIComponent/Components/ListArea.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"

FloatingComponent::FloatingComponent(IRenderer* renderer, TextureResourceBinder* resBinder, const string& mainWndName) noexcept :
	m_renderer{ renderer },
	m_resBinder{ resBinder },
	m_name{ "PopupMenu_" + mainWndName },
	m_component{ nullptr }
{}

FloatingComponent::~FloatingComponent()
{
	Clear();
}

void FloatingComponent::Clear() noexcept
{
	m_component = nullptr;
	m_renderTex.reset();
	m_currentAction.reset();
}

bool FloatingComponent::IsComponent() const noexcept
{
	return m_component != nullptr;
}

unique_ptr<UIComponent> FloatingComponent::GetComponent() noexcept
{
	auto[component, _] = UIEx(m_component).DetachComponent();
	Clear();
	return move(component);
}

void FloatingComponent::SetComponent(unique_ptr<UIComponent> component) noexcept
{
	LoadComponentInternal(move(component), UIEx(component).GetChildrenBoundsSize());
}

bool FloatingComponent::Excute()
{
	if (!m_currentAction.has_value()) return true;

	auto result{ true };
	static const vector<wstring> fontKeys{ L"Hangle", L"English" };

	using enum MakeComponent;
	switch (m_currentAction.value())
	{
	case MPatchTextureStd1: result = LoadComponent(CreateComponent<PatchTextureStd1>(UILayout{ { 64, 64 }, Origin::LeftTop }, "BackImage1")); break;
	case MPatchTextureStd3: result = LoadComponent(CreateComponent<PatchTextureStd3>(UILayout{ { 48, 48 }, Origin::LeftTop }, DirectionType::Horizontal, "ScrollButton3_H_Normal")); break;
	case MPatchTextureStd9: result = LoadComponent(CreateComponent<PatchTextureStd9>(UILayout{ { 170, 120 }, Origin::LeftTop }, "BackImage9")); break;
	case MTextureSwitcher1: result = LoadComponent(CreateComponent<TextureSwitcher>(UILayout{ { 32, 32 }, Origin::LeftTop }, TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal)); break;
	case MTextureSwitcher3: result = LoadComponent(CreateComponent<TextureSwitcher>(UILayout{ { 100, 48 }, Origin::LeftTop }, TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal)); break;
	case MTextureSwitcher9: result = LoadComponent(CreateComponent<TextureSwitcher>(UILayout{ { 100, 100 }, Origin::LeftTop }, TextureSlice::Nine, GetStateKeyMap("ListBackground9"), BehaviorMode::Normal)); break;
	case MTextArea: result = LoadComponent(CreateComponent<TextArea>(UILayout{ { 200, 30 }, Origin::LeftTop }, L"<English><White>Test text.</White></English>", fontKeys)); break;
	case MListArea: result = LoadComponent(CreateSampleListArea({ { 200, 170 }, Origin::LeftTop })); break;
	}
	m_currentAction.reset(); // ���� �ʱ�ȭ

	return result;
}

void FloatingComponent::DrawMakeComponent()
{
	// �ؽ�ó�� �簢�� ���·� �׸���
	m_position = ImGui::GetMousePos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	ImU32 colorU32 = ImGui::GetColorU32(tintColor);

	const auto& texSize = m_renderTex->GetSize();
	drawList->AddImage(
		m_renderTex->GetGraphicMemoryOffset(),
		{ m_position.x, m_position.y },                      // ���� ��ǥ
		{ m_position.x + texSize.x, m_position.y + texSize.y },    // ���� ��ǥ
		ImVec2(0, 0),
		ImVec2(1, 1),
		colorU32
	);
}

void FloatingComponent::Render()
{
	if (IsComponent())
	{
		DrawMakeComponent();
		return;
	}

	// ���콺 ������ ��ư Ŭ�� �� �˾� �޴� ����
	if (!ImGui::BeginPopupContextWindow(m_name.c_str()))
	{
		m_isActive = false;
		return;
	}
	
	m_isActive = true;
	using enum MakeComponent;
	if (ImGui::MenuItem("Patch Texture Standard 1")) m_currentAction = MPatchTextureStd1;
	if (ImGui::MenuItem("Patch Texture Standard 3")) m_currentAction = MPatchTextureStd3;
	if (ImGui::MenuItem("Patch Texture Standard 9")) m_currentAction = MPatchTextureStd9;
	if (ImGui::MenuItem("Texture Switcher 1")) m_currentAction = MTextureSwitcher1;
	if (ImGui::MenuItem("Texture Switcher 3")) m_currentAction = MTextureSwitcher3;
	if (ImGui::MenuItem("Texture Switcher 9")) m_currentAction = MTextureSwitcher9;
	if (ImGui::MenuItem("Text Area")) m_currentAction = MTextArea;
	if (ImGui::MenuItem("List Area")) m_currentAction = MListArea;
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool FloatingComponent::LoadComponentInternal(unique_ptr<UIComponent>&& component, const XMUINT2& size)
{
	ReturnIfFalse(component);
	m_component = component.get();
	ReturnIfFalse(m_renderTex = CreateComponent<RenderTexture>(UILayout{ size, Origin::LeftTop }, move(component)));
	ReturnIfFalse(m_renderTex->BindTextureSourceInfo(m_resBinder, m_renderer->GetTextureController()));
	ReturnIfFalse(m_renderTex->EnableToolMode(true));

	return true;
}

bool FloatingComponent::LoadComponent(unique_ptr<UIComponent> component)
{
	return LoadComponentInternal(move(component), component->GetSize());
}

bool FloatingComponent::ComponentToFloating(unique_ptr<UIComponent>&& component)
{
	return LoadComponentInternal(move(component), UIEx(component).GetChildrenBoundsSize());
}