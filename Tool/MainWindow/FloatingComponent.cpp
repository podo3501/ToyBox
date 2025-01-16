#include "pch.h"
#include "FloatingComponent.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/UIUtility.h"

FloatingComponent::FloatingComponent(IRenderer* renderer, const string& mainWndName) noexcept :
	m_renderer{ renderer },
	m_name{ "PopupMenu_" + mainWndName }
{}

FloatingComponent::~FloatingComponent()
{
	Reset();
}

void FloatingComponent::Reset() noexcept
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
	Reset();
	return move(m_component);
}

bool FloatingComponent::Excute()
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
	if (ImGui::MenuItem("Image Grid 1")) m_currentAction = MakeComponent::ImageGrid1;
	if (ImGui::MenuItem("Image Grid 3")) m_currentAction = MakeComponent::ImageGrid3;
	if (ImGui::MenuItem("Image Grid 9")) m_currentAction = MakeComponent::ImageGrid9;
	//if (ImGui::MenuItem("Close")) {}
	
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

bool FloatingComponent::DetachToFloating(unique_ptr<UIComponent>&& detachComponent)
{
	return LoadComponentInternal(move(detachComponent), detachComponent->GetTotalChildSize());
}


bool FloatingComponent::MakeImageGrid1()
{
	UILayout layout({ 64, 64 }, Origin::LeftTop);
	ImageSource source{ L"UI/SampleTexture/ToolComponentPopup.png", { { 2, 52, 64, 64 } } };
	return LoadComponent(CreateImageGrid<ImageGrid1>("untitled_imageGrid1", layout, source));
}

bool FloatingComponent::MakeImageGrid3()
{
	UILayout layout({ 48, 48 }, Origin::LeftTop);
	ImageSource source{
		L"UI/SampleTexture/ToolComponentPopup.png", {
			{ 2, 2, 23, 48 }, { 25, 2, 2, 48 }, { 27, 2, 23, 48 }
		}
	};
	return LoadComponent(CreateImageGrid<ImageGrid3>("untitled_imageGrid3", layout, source));
}

bool FloatingComponent::MakeImageGrid9()
{
	UILayout layout({ 170, 120 }, Origin::LeftTop);
	ImageSource source{
		L"UI/SampleTexture/ToolComponentPopup.png", {
			{ 2, 52, 30, 36 }, { 32, 52, 4, 36 }, { 36, 52, 30, 36 },
			{ 2, 88, 30, 2 }, { 32, 88, 4, 2 }, { 36, 88, 30, 2 },
			{ 2, 90, 30, 26 }, { 32, 90, 4, 26 }, { 36, 90, 30, 26 }
		}
	};
	return LoadComponent(CreateImageGrid<ImageGrid9>("untitled_imageGrid9", layout, source));
}
