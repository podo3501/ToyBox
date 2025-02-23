#include "pch.h"
#include "FloatingComponent.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Utility.h"

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
	//m_renderer->RemoveRenderComponent(m_component.get()); //�̰� �� ����? ���� ���� �ֳ�?!?
}

bool FloatingComponent::IsComponent() const noexcept
{
	return m_component != nullptr;
}

unique_ptr<UIComponent> FloatingComponent::GetComponent() noexcept
{
	auto[component, _] = UIEx(m_component).DetachComponent();
	m_renderTex.reset();
	m_component = nullptr;
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
	case ImageGrid3: result = LoadComponent(CreateSampleImageGrid3({ { 48, 48 }, Origin::LeftTop })); break;
	case ImageGrid9: result = LoadComponent(CreateSampleImageGrid9({ { 170, 120 }, Origin::LeftTop })); break;
	case Button1: result = LoadComponent(CreateSampleButton1({ { 32, 32 }, Origin::LeftTop })); break;
	case Button3: result = LoadComponent(CreateSampleButton3({ { 100, 48 }, Origin::LeftTop })); break;
	case Text: result = LoadComponent(CreateSampleTextArea({ { 200, 30 }, Origin::LeftTop }, L"<English><White>Test text.</White></English>")); break;

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

	drawList->AddImage(
		m_renderTex->GetGraphicMemoryOffset(),
		{ m_position.x, m_position.y },                      // ���� ��ǥ
		{ m_position.x + m_drawTextureSize.x, m_position.y + m_drawTextureSize.y },    // ���� ��ǥ
		ImVec2(0, 0),
		ImVec2(1, 1),
		colorU32
	);

	//ImGui::SetCursorPos(m_position);
	//ImGui::Image(m_textureID, XMUINT2ToImVec2(m_drawTextureSize),
	//	ImVec2(0, 0),
	//	ImVec2(1, 1),
	//	tintColor);
}

void FloatingComponent::Render()
{
	if (m_draw)
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
	if (ImGui::MenuItem("Image Grid 1")) m_currentAction = ImageGrid1;
	if (ImGui::MenuItem("Image Grid 3")) m_currentAction = ImageGrid3;
	if (ImGui::MenuItem("Image Grid 9")) m_currentAction = ImageGrid9;
	if (ImGui::MenuItem("Button 1")) m_currentAction = Button1;
	if (ImGui::MenuItem("Button 3")) m_currentAction = Button3;
	if (ImGui::MenuItem("Text")) m_currentAction = Text;
	if (ImGui::MenuItem("Close")) {}
	
	ImGui::EndPopup();
}

bool FloatingComponent::LoadComponentInternal(unique_ptr<UIComponent>&& component, const XMUINT2& size)
{
	ReturnIfNullptr(component);
	m_component = component.get();
	ReturnIfFalse(m_renderTex = CreateRenderTexture({ size, Origin::LeftTop }, move(component)));
	ReturnIfFalse(m_renderer->LoadComponent(m_renderTex.get()));

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
	return LoadComponentInternal(move(component), UIEx(component).GetTotalChildSize());
}