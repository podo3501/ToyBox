#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/Window.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "Utility.h"

ToyTestFixture::ToyTestFixture() = default;
ToyTestFixture::~ToyTestFixture() = default;

void ToyTestFixture::SetUp()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	m_window = std::make_unique<Window>();

	HWND hwnd{ 0 };
	RECT rc{0, 0, 800, 600};
	EXPECT_TRUE(m_window->Create(GetModuleHandle(nullptr), SW_HIDE, rc, hwnd));
	const auto& outputSize = m_window->GetOutputSize();
	InitializeConfig(L"ToyTest/TestResources/", outputSize);
	m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), true);

	m_panel = make_unique<Panel>("Main", RectangleToXMUINT2(GetRectResolution()));
	m_renderer->AddRenderComponent(m_panel.get());
}

void ToyTestFixture::TearDown()
{
	//메모리 안 새게 지워준다. 강제로 지우는 이유는 아직 끝나지 않아서 메모리가 남아 있는데
	//ReportLiveObjects 함수가 메모리가 안 지워 졌다고 메세지를 띄우기 때문이다.
	m_renderer.reset();
	m_window.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}