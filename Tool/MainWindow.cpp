#include "pch.h"
#include "MainWindow.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "Utility.h"
#include "../Toy/HelperClass.h"
#include "Popup.h"

MainWindow::~MainWindow()
{
	m_renderer->RemoveRenderTexture(m_textureID);
	m_renderer->RemoveComponent(m_panel.get());
	m_renderer->RemoveImguiComponent(this);
}

ImFont* gfont = nullptr;

MainWindow::MainWindow(IRenderer* renderer) :
	m_renderer{ renderer },
	m_panel{ make_unique<Panel>("Main", GetRectResolution()) },
	m_popup{ make_unique<Tool::Popup>(renderer) },
	m_selectCom{ nullptr }
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
	ReturnIfFalse(ReadJsonFile(filename, m_panel));
	m_renderer->AddComponent(m_panel.get(), false);	//메인 창에는 그리지 않는다.
	ReturnIfFalse(m_renderer->LoadComponents());

	const auto& panelSize = m_panel->GetSize();
	ReturnIfFalse(m_renderer->CreateRenderTexture(panelSize, m_panel.get(), m_textureID));
	m_size = XMUINT2ToImVec2(panelSize);
	m_isOpen = true;

	return true;
}

bool MainWindow::SaveScene(const wstring& filename)
{
	return WriteJsonFile(m_panel, filename);
}

const ImGuiWindow* MainWindow::GetImGuiWindow() const noexcept
{
	return ImGui::FindWindowByName(m_name.c_str());
}

const wstring& MainWindow::GetSaveFilename() const noexcept
{
	return m_panel->GetFilename();
}

bool MainWindow::IsFocus() const noexcept
{
	const ImGuiWindow* window = GetImGuiWindow();
	if (window == nullptr || window->Active == false)
		return false;

	ImGuiWindow* focusedWindow = GImGui->NavWindow;
	if (window != focusedWindow)
		return false;

	return true;
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

void MainWindow::SelectComponent(UIComponent* component) noexcept
{
	if (m_selectCom != nullptr)
		m_selectCom->SetSelected(false);
	
	m_selectCom = component;
	m_selectCom->SetSelected(true);
}

void MainWindow::CheckSelectedComponent(const MouseTracker* mouseTracker) noexcept
{
	if (mouseTracker->leftButton != Mouse::ButtonStateTracker::PRESSED) return;	//왼쪽버튼 눌렀을때 
	if (m_popup->IsComponent())  return;

	static vector<UIComponent*> preComponentList{ nullptr };
	vector<UIComponent*> componentList;
	const XMINT2& pos = mouseTracker->GetOffsetPosition();
	m_panel->GetComponents(pos, componentList);
	if (componentList.empty()) return;

	if (preComponentList == componentList)
	{
		auto findIdx = FindIndex(componentList, m_selectCom);
		if (!findIdx.has_value())
		{
			SelectComponent(componentList.back());
			return;
		}

		int idx = findIdx.value() - 1;
		if (idx < 0) idx = static_cast<int>(componentList.size() - 1);
		SelectComponent(componentList[idx]);
	}
	else //마우스가 다른 컴포넌트를 선택해서 리스트가 바뀌었다면 제일 첫번째 원소를 찍어준다.
	{
		SelectComponent(componentList.back());
		preComponentList = move(componentList);
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

void MainWindow::Update(const DX::StepTimer* timer, MouseTracker* mouseTracker)
{
	if (!IsFocus()) return;
	
	const ImGuiWindow* window = GetImGuiWindow();
	const ImVec2& offset = GetWindowStartPosition(window);
	mouseTracker->SetOffset(ImVec2ToXMINT2(offset));

	//창이 변했을때 RenderTexture를 다시 만들어준다.
	CheckChangeWindow(window, mouseTracker);
	CheckSelectedComponent(mouseTracker);
	CheckAddComponent(mouseTracker);

	m_panel->Update({}, mouseTracker);
	m_popup->Excute(mouseTracker);
}

void MainWindow::ShowTooltip()
{
	const ImGuiWindow* window = GetImGuiWindow();
	const ImVec2& windowMousePos = GetMousePosition(window);

	vector<UIComponent*> componentList;
	m_panel->GetComponents(ImVec2ToXMINT2(windowMousePos), componentList);
	if (componentList.empty()) return;
	
	const ImVec2& padding = ImGui::GetStyle().WindowPadding;
	const ImVec2& mousePos = ImGui::GetMousePos();

	float textY{};
	float curTextY = 20.f;
	for (int idx{ 0 }; !componentList.empty(); ++idx)
	{
		UIComponent* curComponent = componentList.back();
		std::string strIdx = "tooltip_" + std::to_string(idx);
		std::string tooltipContext = curComponent->GetType();

		// 마우스 위치 기반 툴팁 위치 설정
		const ImVec2& tooltipPos = ImVec2(mousePos.x + 20, mousePos.y + curTextY);
		const ImVec2& curSize = ImGui::CalcTextSize(tooltipContext.c_str());
		const ImVec2& tooltipSize = ImVec2(curSize.x + padding.x * 2, curSize.y + padding.y * 2);

		ImGui::SetNextWindowPos(tooltipPos);
		ImGui::SetNextWindowSize(tooltipSize);

		bool selected = (curComponent == m_selectCom);
		if (selected) ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::Begin(strIdx.c_str(), nullptr, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		if (selected) ImGui::PopStyleColor();
		
		ImGui::Text(tooltipContext.c_str());
		ImGui::End();

		curTextY += tooltipSize.y + 5; // 다음 툴팁 위치 업데이트
		componentList.pop_back();
	}
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

	if(!m_popup->IsShowed() && 
		!m_popup->IsComponent() &&
		!ImGui::IsMouseDown(ImGuiMouseButton_Right))
		ShowTooltip();

	ImGui::Image(m_textureID, m_size);
	m_popup->Show();
	
	ImGui::End();
}