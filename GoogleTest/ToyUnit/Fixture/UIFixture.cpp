#include "pch.h"
#include "UIFixture.h"
#include "UserInterface/Helper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/System/Input.h"
#include "Shared/Framework/Environment.h"
#include "Shared/Framework/Locator.h"
#include "Shared/System/Public/IInputManager.h"
#include "Mocks/MockInputManager.h"

using ::testing::_;
using ::testing::Invoke;

UIFixture::~UIFixture() = default;
UIFixture::UIFixture() :
	m_main{ nullptr }
{}

void UIFixture::SetUp()
{
	Rectangle rc{ 0, 0, 800, 600 };
	UILayout layout{ GetSizeFromRectangle(rc), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_mockRenderer = make_unique<MockRenderer>();
	m_mockInputManager = make_unique<MockInputManager>();
	Locator<IInputManager>::Provide(m_mockInputManager.get());
	m_uiModule = CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename);
	m_main = m_uiModule->GetMainPanel();
}

void UIFixture::TearDown()
{
	MockMouseInput(-1, -1, false); //Ű����, ���콺�� stataic Ŭ���� �̱� ������ �����͸� �ʱ�ȭ ��Ų��.
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

//TextArea�� CallMockRender
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
{ //���콺�� ���°��� ������Ʈ�� ����ص� ������ ���°��� ��� ����.
	auto& mouseTracker = const_cast<MouseTracker&>(Input::GetMouse());
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