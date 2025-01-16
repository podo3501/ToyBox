#include "pch.h"
#include "MainWindow.h"
#include "../Utility.h"
#include "../Dialog.h"
#include "FloatingComponent.h"
#include "SelectedComponent/ComponentSelector.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/UIComponentHelper.h"
#include "../Toy/InputManager.h"

int MainWindow::m_mainWindowIndex = 0;

MainWindow::~MainWindow()
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveRenderComponent(m_panel.get());
	m_renderer->RemoveImguiComponent(this);
}

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_name{ "Main Window " + to_string(m_mainWindowIndex++) },
	m_panel{ make_unique<Panel>("Main", RectangleToXMUINT2(GetRectResolution())) },
	m_floater{ make_unique<FloatingComponent>(renderer, m_name) },
	m_selector{ make_unique<ComponentSelector>(renderer, m_panel.get()) }
{
	m_renderer->AddImguiComponent(this);
}

bool MainWindow::CreateScene(const XMUINT2& size)
{
	m_panel->SetSize(size);
	ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(size);
	m_isOpen = true;

	return true;
}

bool MainWindow::CreateScene(const wstring& filename)
{
	ReturnIfFalse(JsonFile::ReadComponent(filename, m_panel));
	ReturnIfFalse(m_renderer->LoadComponent(m_panel.get()));

	const auto& panelSize = m_panel->GetSize();
	ReturnIfFalse(m_renderer->CreateRenderTexture(panelSize, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(panelSize);
	m_isOpen = true;

	m_selector->SetPanel(m_panel.get());

	return true;
}

bool MainWindow::SaveScene(const wstring& filename)
{
	return JsonFile::WriteComponent(m_panel, filename);
}

const ImGuiWindow* MainWindow::GetImGuiWindow() const noexcept
{
	return ImGui::FindWindowByName(m_name.c_str());
}

wstring MainWindow::GetSaveFilename() const noexcept
{
	return JsonFile::GetJsonFilename(m_panel.get());
}

void MainWindow::ChangeWindowSize(const ImVec2& size)
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->CreateRenderTexture(ImVec2ToXMUINT2(size), m_panel.get(), m_textureID);
	m_size = size;
}

void MainWindow::CheckChangeWindow(const ImGuiWindow* window, const MouseTracker& mouseTracker)
{
	static ImVec2 startSize{};
	if (IsInputAction(mouseTracker, MouseButton::Left, KeyState::Pressed))
		startSize = window->Size;

	if (!IsInputAction(mouseTracker, MouseButton::Left, KeyState::Released))
		return;
	
	if(startSize != window->Size && !window->Collapsed)
	{
		ImVec2 newWndSize{ window->Size.x, window->Size.y - GetFrameHeight()};
		ChangeWindowSize(newWndSize);
	}
}

bool MainWindow::CheckAttachComponent(const InputManager& inputManager) noexcept
{
	if (!m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::LeftShift, MouseButton::Left)) return false;
	UIComponent* selectComponent = m_selector->GetComponent();
	if (!selectComponent) return false;
	
	const XMINT2& pos = inputManager.GetMouse().GetOffsetPosition();

	if (selectComponent->EnableAttachment() != AttachmentState::Attach &&
		selectComponent->EnableAttachment() != AttachmentState::All )
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "This component cannot be attached.");
		return true;
	}

	if(!AddComponentFromScreenPos(selectComponent, m_floater->GetComponent(), pos))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Attachment failed for this component.");
		return true;
	}

	return true;
}

bool MainWindow::CheckDetachComponent(const InputManager& inputManager) noexcept
{
	if (m_floater->IsComponent()) return false;
	if (!IsInputAction(inputManager, Keyboard::D, KeyState::Pressed)) return false;
	UIComponent* selectComponent = m_selector->GetComponent();
	if (!selectComponent) return false;
	
	auto detachComponent = selectComponent->DetachComponent();
	if (!detachComponent.has_value())
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Detachment failed for this component.");
		return true;
	}

	//Detach된 Component를 RenderTexture
	if(!m_floater->DetachToFloating(move(detachComponent.value())))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to load the resource.");
		return true;
	}

	m_selector->SetComponent(nullptr);

	return true;
}

void MainWindow::Update(const DX::StepTimer* timer, const InputManager& inputManager)
{
	//if (!IsWindowFocus(m_window)) return;
	if (!m_window) return;

	const ImVec2& offset = GetWindowStartPosition(m_window);
	auto& mouseTracker = const_cast<InputManager&>(inputManager).GetMouse();
	mouseTracker.PushOffset(offset);

	CheckChangeWindow(m_window, mouseTracker); //창이 변했을때 RenderTexture를 다시 만들어준다.
	
	bool isAdd = CheckAttachComponent(inputManager);
	if(!isAdd) 
		m_selector->Update(inputManager);

	m_panel->ProcessUpdate({}, inputManager);
	CheckDetachComponent(inputManager);
	m_floater->Excute();
	mouseTracker.PopOffset();
}

void MainWindow::IgnoreMouseClick() 
{
	const ImVec2& rectMin = GetWindowStartPosition(m_window);
	const ImVec2& rectMax = rectMin + m_size;
	if (!ImGui::IsMouseHoveringRect(rectMin, rectMax)) return;
	
	ImGui::GetIO().MouseDown[0] = false;
}

void MainWindow::SetupWindowAppearing() noexcept
{
	if (!ImGui::IsWindowAppearing()) return;
	
	m_window = const_cast<ImGuiWindow*>(GetImGuiWindow());
	m_selector->SetMainWindow(m_window);
}

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_isOpen)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize({ m_size.x, m_size.y + GetFrameHeight() });
	ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
	//ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

	SetupWindowAppearing();

	ImGui::Image(m_textureID, m_size);

	if (m_window && IsWindowFocus(m_window))
	{
		IgnoreMouseClick();
		Tool::MouseCursor::Render();
	}
	
	if (!Tool::Dialog::IsOpenDialog())
		m_floater->Render();
	m_selector->Render();

	ImGui::End();
}