#include "pch.h"
#include "FloatingComponent.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"

FloatingComponent::FloatingComponent(IRenderer* renderer, const string& mainWndName) noexcept :
	m_renderer{ renderer },
	m_name{ "PopupMenu_" + mainWndName }
{}

FloatingComponent::~FloatingComponent()
{
	Clear();
}

void FloatingComponent::Clear() noexcept
{
	m_drawTextureSize = {};
	m_draw = false;
	m_currentAction.reset();
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveRenderComponent(m_component.get());
	m_textureID = 0;
}

bool FloatingComponent::IsComponent() const noexcept
{
	return m_component != nullptr;
}

unique_ptr<UIComponent> FloatingComponent::GetComponent() noexcept
{
	return move(m_component);
}

void FloatingComponent::SetComponent(unique_ptr<UIComponent> component) noexcept
{
	m_component = move(component);
}

bool FloatingComponent::Excute()
{
	if (!m_currentAction.has_value()) return true;

	auto result{ true };
	using enum MakeComponent;
	switch (m_currentAction.value())
	{
	case ImageGrid1: result = LoadComponent(CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop })); break;
	case ImageGrid3: result = LoadComponent(CreateSampleImageGrid3({ { 48, 48 }, Origin::LeftTop })); break;
	case ImageGrid9: result = LoadComponent(CreateSampleImageGrid9({ { 170, 120 }, Origin::LeftTop })); break;
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

	drawList->AddImage(
		m_textureID,
		{ m_position.x, m_position.y },                      // 시작 좌표
		{ m_position.x + m_drawTextureSize.x, m_position.y + m_drawTextureSize.y },    // 종료 좌표
		ImVec2(0, 0),
		ImVec2(1, 1),
		colorU32
	);
}

void FloatingComponent::Render()
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

	using enum MakeComponent;
	if (ImGui::MenuItem("Image Grid 1")) m_currentAction = ImageGrid1;
	if (ImGui::MenuItem("Image Grid 3")) m_currentAction = ImageGrid3;
	if (ImGui::MenuItem("Image Grid 9")) m_currentAction = ImageGrid9;
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool FloatingComponent::IsActive() const noexcept
{
	return m_isActive;
}

bool FloatingComponent::LoadComponentInternal(unique_ptr<UIComponent>&& component, const XMUINT2& size)
{
	ReturnIfNullptr(component);

	ImTextureID texID{};
	ReturnIfFalse(m_renderer->CreateRenderTexture(size, component.get(), texID));
	ReturnIfFalse(m_renderer->LoadComponent(component.get()));

	m_textureID = texID;
	m_component = move(component);
	m_drawTextureSize = size;
	m_draw = true;

	return true;
}

bool FloatingComponent::LoadComponent(unique_ptr<UIComponent>&& component)
{
	return LoadComponentInternal(move(component), component->GetSize());
}

bool FloatingComponent::ComponentToFloating(unique_ptr<UIComponent>&& component)
{
	return LoadComponentInternal(move(component), component->GetTotalChildSize());
}