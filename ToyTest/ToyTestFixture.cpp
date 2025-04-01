#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/Window.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/Command/CommandList.h"
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
	m_sourceBinder = CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json");
	m_renderer->LoadTextureBinder(m_sourceBinder.get());
}

using ::testing::_;
using ::testing::Invoke;

void ToyTestFixture::CallMockRender(UIComponent* component, function<void(size_t, const RECT&, const RECT*, TextureSourceBinder*)> testRenderFunc, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke([this, testRenderFunc](size_t index, const RECT& dest, const RECT* source) {
		testRenderFunc(index, dest, source, m_sourceBinder.get());
			}));
	component->ProcessUpdate(m_timer);
	component->ProcessRender(&mockRender);
}

void ToyTestFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureSourceBinder*)> testRenderFunc, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke([this, testRenderFunc](size_t index, const RECT& dest, const RECT* source) {
		testRenderFunc(index, dest, source, m_sourceBinder.get());
			}));
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

void ToyTestFixture::MockMouseInput(int mouseX, int mouseY, bool leftButton)
{ //마우스의 상태값은 업데이트를 계속해도 셋팅한 상태값이 계속 들어간다.
	auto& mouseTracker = const_cast<MouseTracker&>(InputManager::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	state.leftButton = leftButton;
	mouseTracker.Update(state);
}

void ToyTestFixture::CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*, TextureSourceBinder*)> renderFunc, int times)
{
	unique_ptr<UIComponent> clonePanel = component->Clone();

	CallMockRender(clonePanel.get(), renderFunc, times);
	WriteReadTest(clonePanel);
}

void ToyTestFixture::TearDown()
{
	MockMouseInput(-1, -1, false); //키보드, 마우스는 stataic 클래스 이기 때문에 데이터를 초기화 시킨다.
	//메모리 안 새게 지워준다. 강제로 지우는 이유는 아직 끝나지 않아서 메모리가 남아 있는데
	//ReportLiveObjects 함수가 메모리가 안 지워 졌다고 메세지를 띄우기 때문이다.
	m_panel.reset();
	m_sourceBinder.reset();
	m_renderer.reset();
	m_window.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

bool IntegrationTest::VerifyClone(unique_ptr<UIComponent> original)
{
	if (!original) return false;
	EXPECT_TRUE(original->BindTextureSourceInfo(m_sourceBinder.get(), m_renderer->GetTextureController()));
	auto clone = original->Clone();

	return CompareUniquePtr(original, clone);
}

void IntegrationTest::CaptureSnapshot(CommandList& cmdList, vector<unique_ptr<UIComponent>>& history)
{
	history.emplace_back(m_panel->Clone());
}

void IntegrationTest::VerifyUndoRedo(CommandList& cmdList, const vector<unique_ptr<UIComponent>>& history)
{
	for_each(history.rbegin() + 1, history.rend(), [&](const auto& snapshot) {
		cmdList.Undo();
		EXPECT_TRUE(CompareUniquePtr(m_panel, snapshot));
		});

	for_each(history.begin() + 1, history.end(), [&](const auto& snapshot) {
		cmdList.Redo();
		EXPECT_TRUE(CompareUniquePtr(m_panel, snapshot));
		});
}