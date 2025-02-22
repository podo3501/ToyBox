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
#include "IMockRenderer.h"
#include "TestHelper.h"

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

using ::testing::_;
using ::testing::Invoke;

void ToyTestFixture::CallMockRender(UIComponent* component, function<void(size_t, const RECT&, const RECT*)> testRenderFunc, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessUpdate(m_timer);
	component->ProcessRender(&mockRender);
}

void ToyTestFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*)> testRenderFunc, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke(testRenderFunc));
	m_panel->ProcessUpdate(m_timer);
	m_panel->ProcessRender(&mockRender);
}

void ToyTestFixture::CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessUpdate(m_timer);
	component->ProcessRender(&mockRender);
}

void ToyTestFixture::CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	m_panel->ProcessUpdate(m_timer);
	m_panel->ProcessRender(&mockRender);
}

void ToyTestFixture::TestUpdate(int mouseX, int mouseY)
{
	m_panel->ProcessUpdate(m_timer);
	auto& mouseTracker = const_cast<MouseTracker&>(InputManager::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	mouseTracker.Update(state);
}

void ToyTestFixture::CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*)> renderFunc, int times)
{
	unique_ptr<UIComponent> clonePanel = component->Clone();

	CallMockRender(clonePanel.get(), renderFunc, times);
	EXPECT_TRUE(WriteReadTest(clonePanel));
}

void ToyTestFixture::TearDown()
{
	//�޸� �� ���� �����ش�. ������ ����� ������ ���� ������ �ʾƼ� �޸𸮰� ���� �ִµ�
	//ReportLiveObjects �Լ��� �޸𸮰� �� ���� ���ٰ� �޼����� ���� �����̴�.
	m_panel.reset();
	m_renderer.reset();
	m_window.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}