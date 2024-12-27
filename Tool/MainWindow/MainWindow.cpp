#include "pch.h"
#include "MainWindow.h"
#include "../Utility.h"
#include "ComponentPopup.h"
#include "ComponentTooltip.h"
#include "ComponentWindow.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/HelperClass.h"
#include "../Toy/InputManager.h"

MainWindow::~MainWindow()
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveRenderComponent(m_panel.get());
	m_renderer->RemoveImguiComponent(this);
}

ImFont* gfont = nullptr;

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_panel{ make_unique<Panel>("Main", RectangleToXMUINT2(GetRectResolution())) },
	m_popup{ make_unique<ComponentPopup>(renderer) },
	m_comWindow{ nullptr },
	m_tooltip{ make_unique<ComponentTooltip>(m_panel.get()) }
{
	static int idx{ 0 };
	m_name = "Main Window " + to_string(idx++);
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
	m_tooltip->SetPanel(m_panel.get());
	ReturnIfFalse(m_renderer->LoadComponent(m_panel.get()));

	const auto& panelSize = m_panel->GetSize();
	ReturnIfFalse(m_renderer->CreateRenderTexture(panelSize, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(panelSize);
	m_isOpen = true;

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

void MainWindow::CheckChangeWindow(const ImGuiWindow* window, const MouseTracker* mouseTracker)
{
	static ImVec2 startSize{};
	if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
		startSize = window->Size;

	if (mouseTracker->leftButton != Mouse::ButtonStateTracker::RELEASED)
		return;
	
	if(startSize != window->Size && !window->Collapsed)
	{
		ImVec2 newWndSize{ window->Size.x, window->Size.y - GetFrameHeight()};
		ChangeWindowSize(newWndSize);
	}
}

void MainWindow::CheckAddComponent(const MouseTracker* mouseTracker) noexcept
{
	if (mouseTracker->leftButton != Mouse::ButtonStateTracker::PRESSED) return;	//왼쪽버튼 눌렀을때 
	if (!m_popup->IsComponent())  return;

	const XMUINT2 size = m_panel->GetSize();
	const XMINT2& pos = mouseTracker->GetOffsetPosition();
	m_panel->AddComponent(m_popup->GetComponent(), GetNormalPosition(pos, size));
}

static unique_ptr<ComponentWindow> CreateComponentWindow(const UIComponent* component, IRenderer* renderer)
{
	const string& type = component->GetType();

	unique_ptr<ComponentWindow> componentWindow{ nullptr };
	if (type == "class Panel") componentWindow = make_unique<ComponentPanel>();
	if (type == "class ImageGrid1") componentWindow = make_unique<ComponentImageGrid1>();
	if (type == "class ImageGrid3") componentWindow = make_unique<ComponentImageGrid3>();
	if (type == "class ImageGrid9") componentWindow = make_unique<ComponentImageGrid9>();

	componentWindow->SetRenderer(renderer);

	return move(componentWindow);
}

void MainWindow::CheckSelectedComponent(InputManager* inputManager)
{
	if (m_popup->IsActive()) return;

	m_tooltip->CheckSelectedComponent(inputManager);
	UIComponent* component = m_tooltip->GetComponent();
	if (component == nullptr) return;

	const UIComponent* winComponent = (m_comWindow != nullptr) ? m_comWindow->GetComponent() : nullptr;
	if (winComponent == component) return;

	m_comWindow = CreateComponentWindow(component, m_renderer);
	m_comWindow->SetComponent(component);
}

void MainWindow::Update(const DX::StepTimer* timer, InputManager* inputManager)
{
	if (!IsWindowFocus(m_window)) return;

	const ImVec2& offset = GetWindowStartPosition(m_window);
	auto mouseTracker = inputManager->GetMouse();
	mouseTracker->SetOffset(ImVec2ToXMINT2(offset));

	CheckChangeWindow(m_window, mouseTracker); //창이 변했을때 RenderTexture를 다시 만들어준다.
	CheckAddComponent(mouseTracker);
	CheckSelectedComponent(inputManager);

	m_panel->ProcessUpdate({}, inputManager);
	m_popup->Excute(mouseTracker);

	if (m_comWindow != nullptr)
		m_comWindow->Update();
}

void MainWindow::RenderMain()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize({ m_size.x, m_size.y + GetFrameHeight() });
	ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
	//ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.
	if(ImGui::IsWindowAppearing())
		m_window = const_cast<ImGuiWindow*>(GetImGuiWindow());

	ImGui::Image(m_textureID, m_size);

	m_popup->Render();
	m_tooltip->Render(m_window);

	ImGui::End();
}

void MainWindow::Render(ImGuiIO* io)
{
	if (!m_isOpen)
		return;

	RenderMain();
	if (m_comWindow != nullptr)
		m_comWindow->Render();
}