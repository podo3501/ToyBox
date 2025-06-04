#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/Window.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/Command/TexResCommandList/TexResCommandList.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/UserInterface/UIModule.h"
#include "../Toy/InputManager.h"
#include "../Toy/Utility.h"
#include "Utility.h"
#include "IMockRenderer.h"
#include "TestHelper.h"

ToyTestFixture::~ToyTestFixture() = default;
ToyTestFixture::ToyTestFixture() :
	m_main{ nullptr }
{}

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

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	m_uiModule = CreateUIModule(layout, "Main", m_renderer.get(), srcBinderFilename);
	m_main = m_uiModule->GetComponent();

	TracyStartupProfiler();
}

void ToyTestFixture::TearDown()
{
	TracyShutdownProfiler();

	MockMouseInput(-1, -1, false); //키보드, 마우스는 stataic 클래스 이기 때문에 데이터를 초기화 시킨다.
	//메모리 안 새게 지워준다. 강제로 지우는 이유는 아직 끝나지 않아서 메모리가 남아 있는데
	//ReportLiveObjects 함수가 메모리가 안 지워 졌다고 메세지를 띄우기 때문이다.
	m_uiModule.reset();
	m_renderer.reset();
	m_window.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

using ::testing::_;
using ::testing::Invoke;

void ToyTestFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke([this, testRenderFunc](size_t index, const RECT& dest, const RECT* source) {
		testRenderFunc(index, dest, source, GetResBinder());
			}));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockRender);
}

void ToyTestFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, const vector<RECT>&)> testRenderFunc, 
	const string& bindKey, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke([this, testRenderFunc, &bindKey](size_t index, const RECT& dest, const RECT* source) {
		testRenderFunc(index, dest, source, GetSources(GetResBinder(), bindKey));
			}));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockRender);
}

void ToyTestFixture::TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(static_cast<int>(expectDest.size()))
		.WillRepeatedly(Invoke([this, expIndex, &expectDest, &bindKey](size_t index, const RECT& dest, const RECT* source) {
		TestCoordinates(index, dest, source, expIndex, expectDest, GetSources(GetResBinder(), bindKey));
			}));
	UIComponent* curComponent = (component) ? component : m_main;
	curComponent->ProcessUpdate(m_timer);
	curComponent->ProcessRender(&mockRender);
}

//TextArea용 CallMockRender
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
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockRender);
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

void ToyTestFixture::CloneTest(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(clonePanel);
}

void ToyTestFixture::CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(GetResBinder(), clonePanel);
	//WriteReadTest(m_resBinder.get(), clonePanel);
}
TextureResourceBinder* ToyTestFixture::GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

bool IntegrationTest::VerifyClone(unique_ptr<UIComponent> original)
{
	if (!original) return false;
	auto resBinder = m_uiModule->GetTexResBinder();
	EXPECT_TRUE(original->BindTextureSourceInfo(resBinder, m_renderer->GetTextureController()));
	auto clone = original->Clone();

	return CompareUniquePtr(original, clone);
}

void TextureSwitcherComponentTest::FitToTextureSourceTest(const string& bindingKey)
{
	TextureSwitcher* texSwitcher = UIEx(m_main).FindComponent<TextureSwitcher*>("TextureSwitcher_0");
	EXPECT_TRUE(texSwitcher->FitToTextureSource());
	EXPECT_EQ(texSwitcher->GetSize(), GetSizeOfBindKey(GetResBinder(), bindingKey));
}

void UndoRedoTest::CaptureSnapshot(vector<unique_ptr<UIComponent>>& history)
{
	history.emplace_back(m_main->Clone());
}

void UndoRedoTest::CaptureSnapshot(vector<unique_ptr<TextureResourceBinder>>& history)
{
	history.emplace_back(make_unique<TextureResourceBinder>(*GetResBinder()));
}

void UndoRedoTest::VerifyUndoRedo(UICommandList& cmdList, const vector<unique_ptr<UIComponent>>& history)
{
	for_each(history.rbegin() + 1, history.rend(), [&](const auto& snapshot) {
		cmdList.Undo();
		EXPECT_TRUE(*m_main == *snapshot);
		});

	for_each(history.begin() + 1, history.end(), [&](const auto& snapshot) {
		cmdList.Redo();
		EXPECT_TRUE(*m_main == *snapshot);
		});
}

void UndoRedoTest::VerifyUndoRedo(TexResCommandList& cmdList, const vector<unique_ptr<TextureResourceBinder>>& history)
{
	auto resBinder = GetResBinder();

	for_each(history.rbegin() + 1, history.rend(), [&](const auto& snapshot) {
		cmdList.Undo();
		EXPECT_TRUE(*resBinder == *snapshot);
		});

	for_each(history.begin() + 1, history.end(), [&](const auto& snapshot) {
		cmdList.Redo();
		EXPECT_TRUE(*resBinder == *snapshot);
		});
}

