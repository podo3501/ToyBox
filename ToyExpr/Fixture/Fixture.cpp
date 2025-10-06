#include "pch.h"
#include "Fixture.h"
#include "IRenderer.h"
#include "Shared/Utils/Profiler.h"
#include "Shared/Window/Window.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/System/Input.h"
#include "Shared/Framework/Environment.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIModule.h"

Fixture::~Fixture() = default;
Fixture::Fixture() :
	m_main{ nullptr }
{
}

void Fixture::SetUp()
{
	m_window = std::make_unique<Window>();

	HWND hwnd{ 0 };
	RECT rc{ 0, 0, 800, 600 };
	EXPECT_TRUE(m_window->Create(GetModuleHandle(nullptr), SW_HIDE, rc, hwnd));
	const auto& outputSize = m_window->GetOutputSize();
	InitializeEnvironment(L"Resources/", outputSize);
	m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), true);
	Input::Initialize(hwnd);

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	m_uiModule = CreateUIModule(layout, "Main", m_renderer.get(), srcBinderFilename);
	m_uiModule->AddRenderer();
	m_main = m_uiModule->GetMainPanel();

	TracyStartupProfiler();
}

void Fixture::TearDown()
{
	TracyShutdownProfiler();

	MockMouseInput(-1, -1, false); //키보드, 마우스는 stataic 클래스 이기 때문에 데이터를 초기화 시킨다.
}

TextureResourceBinder* Fixture::GetResBinder() const noexcept
{ 
	return m_uiModule->GetTexResBinder(); 
}

void Fixture::MockMouseInput(int mouseX, int mouseY, bool leftButton)
{ //마우스의 상태값은 업데이트를 계속해도 셋팅한 상태값이 계속 들어간다.
	auto& mouseTracker = const_cast<MouseTracker&>(Input::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	state.leftButton = leftButton;
	mouseTracker.Update(state);
}