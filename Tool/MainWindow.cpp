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
	m_panel{ make_unique<Panel>("Main", GetRectResolution())},
	m_popup{ make_unique<Tool::Popup>( renderer ) }
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

void MainWindow::CheckChangeWindow(const ImGuiWindow* window, MouseTracker* mouseTracker)
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

void MainWindow::Update(const DX::StepTimer* timer, MouseTracker* mouseTracker)
{
	if (!IsFocus()) return;
	
	const ImGuiWindow* window = GetImGuiWindow();
	const ImVec2& offset = GetWindowStartPosition(window);
	mouseTracker->SetOffset(ImVec2ToXMINT2(offset));

	//창이 변했을때 RenderTexture를 다시 만들어준다.
	CheckChangeWindow(window, mouseTracker);

	if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		if(m_popup->IsComponent())
		{
			const XMUINT2 size = m_panel->GetSize();
			const ImVec2& pos = GetMousePosition(window);

			m_panel->AddComponent(m_popup->GetComponent(), GetNormalPosition(pos, size));
		}
	}

	m_panel->Update({}, mouseTracker);
	m_popup->Excute(mouseTracker);
}

void MainWindow::ShowTooltip()
{
	const ImGuiWindow* window = GetImGuiWindow();
	const ImVec2& mousePos = GetMousePosition(window);

	vector<const UIComponent*> componentList;
	m_panel->GetComponents(ImVec2ToXMINT2(mousePos), componentList);
	if (componentList.empty()) return;

	//vector<string> tooltipList;
	//tooltipList.push_back("tooltip 1");
	//tooltipList.push_back("tooltip 2");
	//tooltipList.push_back("tooltip 3");

	static int selected{ 0 };
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) 
	{
		++selected;
		if (selected >= componentList.size()) selected = 0;
	}

	//if (!ImGui::IsItemHovered()) return;
	
	ImVec2 padding{};
	padding = ImGui::GetStyle().WindowPadding;

	float textY{};
	float curTextY = 20.f;
	for (int idx{ 0 }; idx != componentList.size(); ++idx)
	{
		string strIdx = "tooltip_" + to_string(idx);
		float y = static_cast<float>(idx) * 40;

		const ImVec2& mousePos = ImGui::GetMousePos();
		ImVec2 tooltipPos = ImVec2(mousePos.x + 20, mousePos.y + curTextY);
		ImGui::SetNextWindowPos(tooltipPos);

		string tooltipContext = componentList[idx]->GetType();
		ImVec2 curSize = ImGui::CalcTextSize(tooltipContext.c_str());
		ImVec2 tooltipSize = ImVec2(curSize.x + padding.x * 2, curSize.y + padding.y * 2);
		ImGui::SetNextWindowSize(tooltipSize);

		if (selected == idx) ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::Begin(strIdx.c_str(), nullptr, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		if (selected == idx) ImGui::PopStyleColor();
		
		ImGui::Text(tooltipContext.c_str());
		ImVec2 curGapSize = ImGui::CalcTextSize(tooltipContext.c_str());
		curTextY += padding.y * 2 + curGapSize.y + 5;
		ImGui::End();
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

	//ImVec2 windowPos = ImGui::GetWindowPos();
		// 첫 번째 툴팁
	//if (ImGui::Button("Hover me"))
	{
		// 버튼 클릭 시 실행될 코드
	}
	
	//ShowTooltip(GetComponent());
	ShowTooltip();
	//if (ImGui::IsItemHovered())
	//{
	//	//// 첫 번째 툴팁
	//	//ImVec2 tooltipPos = ImVec2(ImGui::GetMousePos().x + 10, ImGui::GetMousePos().y + 20); // 마우스 위치에서 20픽셀 아래, 오른쪽에 툴팁 표시
	//	//ImGui::SetNextWindowPos(tooltipPos, ImGuiCond_None); // 툴팁의 위치를 설정

	//	//ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.f, 0.0f, 0.0f, 1.0f));
	//	//ImGui::BeginTooltip();
	//	//ImGui::PopStyleColor();
	//	////ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // 빨간색 테두리
	//	////ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); // 패딩 설정
	//	////ImGui::InvisibleButton("Rect1", ImVec2(10, 10)); // 첫 번째 사각형
	//	////if (ImGui::IsItemHovered()) {

	//	//ImFont* font1 = io->Fonts->Fonts[1];
	//	//ImGui::PushFont(font1);
	//	//ImGui::Text("This is tooltip 1");
	//	//ImVec2 text_size = ImGui::CalcTextSize("This is tooltip 1");
	//	//ImGui::PopFont();
	//	//ImGui::Text("This is tooltip 2");
	//	//ImVec2 text_size2 = ImGui::CalcTextSize("This is tooltip 2");
	//	//ImVec2 padding = ImGui::GetStyle().WindowPadding;

	//	////}
	//	////ImGui::PopStyleVar();
	//	////ImGui::PopStyleColor();
	//	//ImGui::EndTooltip();

	//	//ImVec2 tooltip_size = ImVec2(text_size.x + text_size2.x + padding.x * 2, text_size.y + text_size2.y + padding.y * 2);

	//	//tooltipPos = ImVec2(ImGui::GetMousePos().x + 10, ImGui::GetMousePos().y + 160); // 마우스 위치에서 20픽셀 아래, 오른쪽에 툴팁 표시
	//	//ImGui::SetNextWindowPos(tooltipPos, ImGuiCond_None); // 툴팁의 위치를 설정

	//	//ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.f, 0.0f, 0.0f, 1.0f));
	//	//ImGui::BeginTooltip();
	//	//ImGui::PopStyleColor();

	//	//ImGui::Text("This is tooltip 3");

	//	//ImGui::EndTooltip();

	//	//ImGui::SetNextWindowPos(ImGui::GetMousePos() + ImVec2(20, 20)); // 마우스 기준 오프셋
	//	ImVec2 tooltipPos = ImVec2(ImGui::GetMousePos().x + 10, ImGui::GetMousePos().y + 20); // 마우스 위치에서 20픽셀 아래, 오른쪽에 툴팁 표시
	//	ImGui::SetNextWindowPos(tooltipPos);
	//	ImGui::Begin("Custom Tooltip 1", nullptr, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	//	ImGui::Text("This is the second tooltip!");
	//	ImGui::End();

	//	tooltipPos = ImVec2(ImGui::GetMousePos().x + 10, ImGui::GetMousePos().y + 60); // 마우스 위치에서 20픽셀 아래, 오른쪽에 툴팁 표시
	//	ImGui::SetNextWindowPos(tooltipPos);
	//	ImGui::Begin("Custom Tooltip 2", nullptr, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	//	ImGui::Text("This is the second tooltip!");
	//	ImGui::End();
	//}


	ImGui::Image(m_textureID, m_size);
	m_popup->Show();
	
	ImGui::End();
}