#include "pch.h"
#include "FloatingComponent.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Component/ImageSwitcher.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Utility.h"

FloatingComponent::FloatingComponent(IRenderer* renderer, TextureSourceBinder* sourceBinder, const string& mainWndName) noexcept :
	m_renderer{ renderer },
	m_sourceBinder{ sourceBinder },
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
	LoadComponentInternal(move(component), UIEx(component).GetTotalChildSize());
}

bool FloatingComponent::Excute()
{
	if (!m_currentAction.has_value()) return true;

	auto result{ true };
	using enum MakeComponent;
	switch (m_currentAction.value())
	{
	case ImageGrid1: result = LoadComponent(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop })); break;
	case ImageGrid3: result = LoadComponent(CreateSampleImageGrid3(DirectionType::Horizontal, { { 48, 48 }, Origin::LeftTop })); break;
	case ImageGrid9: result = LoadComponent(CreateSampleImageGrid9({ { 170, 120 }, Origin::LeftTop })); break;
	case ImageSwitcher1: result = LoadComponent(CreateSampleImageSwitcher1({ { 32, 32 }, Origin::LeftTop }, BehaviorMode::Normal)); break;
	case ImageSwitcher3: result = LoadComponent(CreateSampleImageSwitcher3({ { 100, 48 }, Origin::LeftTop }, DirectionType::Horizontal, BehaviorMode::Normal)); break;
	case Button1: result = LoadComponent(CreateSampleButton1({ { 32, 32 }, Origin::LeftTop })); break;
	case Button3: result = LoadComponent(CreateSampleButton3(DirectionType::Horizontal, { { 100, 48 }, Origin::LeftTop })); break;
	case Text: result = LoadComponent(CreateSampleTextArea({ { 200, 30 }, Origin::LeftTop }, L"<English><White>Test text.</White></English>")); break;
	case ListArea: result = LoadComponent(CreateSampleListArea1({ { 200, 170 }, Origin::LeftTop })); break;
	}
	m_currentAction.reset(); // 상태 초기화

	return result;
}

void FloatingComponent::DrawMakeComponent()
{
	// 텍스처를 사각형 형태로 그리기
	m_position = ImGui::GetMousePos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	ImU32 colorU32 = ImGui::GetColorU32(tintColor);

	const auto& texSize = m_renderTex->GetSize();
	drawList->AddImage(
		m_renderTex->GetGraphicMemoryOffset(),
		{ m_position.x, m_position.y },                      // 시작 좌표
		{ m_position.x + texSize.x, m_position.y + texSize.y },    // 종료 좌표
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

	// 마우스 오른쪽 버튼 클릭 시 팝업 메뉴 띄우기
	if (!ImGui::BeginPopupContextWindow(m_name.c_str()))
	{
		m_isActive = false;
		return;
	}
	
	m_isActive = true;

	using enum MakeComponent;
	if (ImGui::MenuItem("Image Grid 1")) m_currentAction = ImageGrid1;
	if (ImGui::MenuItem("Image Grid 3")) m_currentAction = ImageGrid3;
	if (ImGui::MenuItem("Image Grid 9")) m_currentAction = ImageGrid9;
	if (ImGui::MenuItem("Image Switcher 1")) m_currentAction = ImageSwitcher1;
	if (ImGui::MenuItem("Image Switcher 3")) m_currentAction = ImageSwitcher3;
	if (ImGui::MenuItem("Button 1")) m_currentAction = Button1;
	if (ImGui::MenuItem("Button 3")) m_currentAction = Button3;
	if (ImGui::MenuItem("Text")) m_currentAction = Text;
	if (ImGui::MenuItem("ListArea")) m_currentAction = ListArea;
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool FloatingComponent::LoadComponentInternal(unique_ptr<UIComponent>&& component, const XMUINT2& size)
{
	ReturnIfNullptr(component);
	m_component = component.get();
	ReturnIfFalse(m_renderTex = CreateRenderTexture({ size, Origin::LeftTop }, move(component)));
	ReturnIfFalse(m_renderer->LoadComponent(m_renderTex.get()));

	return true;
}

bool FloatingComponent::LoadComponent(unique_ptr<UIComponent> component)
{
	return LoadComponentInternal(move(component), component->GetSize());
}

bool FloatingComponent::ComponentToFloating(unique_ptr<UIComponent>&& component)
{
	return LoadComponentInternal(move(component), UIEx(component).GetTotalChildSize());
}