#include "pch.h"
#include "UIFixture.h"
#include "GameConfig.h"
#include "Utils/CommonUtil.h"
#include "Utils/GeometryUtil.h"
#include "InputManager.h"
#include "Utility.h"
#include "Utils/StlUtil.h"
#include "UserInterface/UIModule.h"
#include "UserInterface/UIComponent/UIType.h"
#include "UserInterface/UIComponent/UILayout.h"
#include "UserInterface/TextureResourceBinder/TextureBinderHelper.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/Command/UICommandList/UICommandList.h"
#include "UserInterface/Command/TexResCommandList/TexResCommandList.h"
#include "UserInterface/Helper.h"

using ::testing::_;
using ::testing::Invoke;

UIFixture::~UIFixture() = default;
UIFixture::UIFixture() :
	m_main{ nullptr }
{}

void UIFixture::SetUp()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Rectangle rc{ 0, 0, 800, 600 };
	UILayout layout{ GetSizeFromRectangle(rc), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	InitializeConfig(L"Resources/", { 800.f, 600.f });
	m_mockRenderer = make_unique<MockRenderer>();
	m_uiModule = CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename);
	m_main = m_uiModule->GetComponent();

	TracyStartupProfiler();
}

void UIFixture::TearDown()
{
	TracyShutdownProfiler();

	MockMouseInput(-1, -1, false); //키보드, 마우스는 stataic 클래스 이기 때문에 데이터를 초기화 시킨다.
	//메모리 안 새게 지워준다. 강제로 지우는 이유는 아직 끝나지 않아서 메모리가 남아 있는데
	//ReportLiveObjects 함수가 메모리가 안 지워 졌다고 메세지를 띄우기 때문이다.

	m_main = nullptr;
	m_uiModule.reset();
	m_mockRenderer.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

TextureResourceBinder* UIFixture::GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

void UIFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke([this, testRenderFunc](size_t index, const RECT& dest, const RECT* source) {
		testRenderFunc(index, dest, source, GetResBinder());
			}));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockTexRender);
}

void UIFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, const vector<RECT>&)> testRenderFunc,
	const string& bindKey, int times)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke([this, testRenderFunc, &bindKey](size_t index, const RECT& dest, const RECT* source) {
		testRenderFunc(index, dest, source, GetSources(GetResBinder(), bindKey));
			}));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockTexRender);
}

void UIFixture::TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, Render(_, _, _))
		.Times(static_cast<int>(expectDest.size()))
		.WillRepeatedly(Invoke([this, expIndex, &expectDest, &bindKey](size_t index, const RECT& dest, const RECT* source) {
		TestCoordinates(index, dest, source, expIndex, expectDest, GetSources(GetResBinder(), bindKey));
			}));
	UIComponent* curComponent = (component) ? component : m_main;
	curComponent->ProcessUpdate(m_timer);
	curComponent->ProcessRender(&mockTexRender);
}

//TextArea용 CallMockRender
void UIFixture::CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessUpdate(m_timer);
	component->ProcessRender(&mockTexRender);
}

void UIFixture::CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockTexRender);
}

void UIFixture::MockMouseInput(int mouseX, int mouseY, bool leftButton)
{ //마우스의 상태값은 업데이트를 계속해도 셋팅한 상태값이 계속 들어간다.
	auto& mouseTracker = const_cast<MouseTracker&>(InputManager::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	state.leftButton = leftButton;
	mouseTracker.Update(state);
}

void UIFixture::CloneTest(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(clonePanel);
}

//////////////////////////////////////////////////////////////////

void TextureSwitcherComponent::FitToTextureSourceTest(const string& bindingKey)
{
	TextureSwitcher* texSwitcher = UIEx(m_main).FindComponent<TextureSwitcher*>("TextureSwitcher");
	EXPECT_TRUE(texSwitcher->FitToTextureSource());
	EXPECT_EQ(texSwitcher->GetSize(), GetSizeOfBindKey(GetResBinder(), bindingKey));
}

void TextureSwitcherComponent::CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(GetResBinder(), clonePanel);
	//WriteReadTest(m_resBinder.get(), clonePanel);
}

//////////////////////////////////////////////////////////////////

bool Integration::VerifyClone(unique_ptr<UIComponent> original)
{
	if (!original) return false;
	auto resBinder = m_uiModule->GetTexResBinder();
	EXPECT_TRUE(original->BindTextureSourceInfo(resBinder, m_mockRenderer->GetTextureController()));
	auto clone = original->Clone();

	return Compare(original, clone);
}

//////////////////////////////////////////////////////////////////

void UndoRedo::CaptureSnapshot(vector<unique_ptr<UIComponent>>& history)
{
	history.emplace_back(m_main->Clone());
}

void UndoRedo::CaptureSnapshot(vector<unique_ptr<TextureResourceBinder>>& history)
{
	history.emplace_back(make_unique<TextureResourceBinder>(*GetResBinder()));
}

void UndoRedo::VerifyUndoRedo(UICommandList& cmdList, const vector<unique_ptr<UIComponent>>& history)
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

void UndoRedo::VerifyUndoRedo(TexResCommandList& cmdList, const vector<unique_ptr<TextureResourceBinder>>& history)
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