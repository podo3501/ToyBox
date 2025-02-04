#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/Window.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/InputManager.h"
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
	InitializeConfig(L"Resources/", outputSize);
	m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), true);
	InputManager::Initialize(hwnd);
	UILayout layout{ RectangleToXMUINT2(GetRectResolution()), Origin::LeftTop };
	m_panel = CreateRootPanel("Main", layout, m_renderer.get());
}

void ToyTestFixture::TearDown()
{
	//�޸� �� ���� �����ش�. ������ ����� ������ ���� ������ �ʾƼ� �޸𸮰� ���� �ִµ�
	//ReportLiveObjects �Լ��� �޸𸮰� �� ���� ���ٰ� �޼����� ���� �����̴�.
	m_renderer.reset();
	m_window.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}