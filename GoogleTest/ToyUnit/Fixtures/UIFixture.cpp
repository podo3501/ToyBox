#include "pch.h"
#include "UIFixture.h"
#include "Tests/Helper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/Locator/InputLocator.h"
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
	m_environment = InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_mockRenderer = make_unique<MockRenderer>();
	m_mockRenderer->RegisterMockTextureInfos({
		{L"../Resources/UI/Font/CourierNewBoldS18.spritefont", 0, {}},
		{L"../Resources/UI/Font/MaleunGothicS16.spritefont", 1, {}},
		{L"../Resources/UI/SampleTexture/Sample_0.png", 2, {512, 512}},
		{L"../Resources/UI/SampleTexture/Option.png", 3, {512, 512}} });
	m_mockInput = make_unique<MockInputManager>();
	InputLocator::Provide(m_mockInput.get());
	m_uiModule = CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename);
	m_main = m_uiModule->GetMainPanel();
}

void UIFixture::TearDown()
{}

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
	//curComponent->ProcessUpdate(m_timer);
	m_uiModule->Update(m_timer);
	curComponent->ProcessRender(&mockTexRender);
}

//TextArea¿ë CallMockRender
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

void UIFixture::CloneTest(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(clonePanel);
}

void UIFixture::SimulateMouse(const XMINT2& pos, InputState state) noexcept
{
	SimulateMouse(pos.x, pos.y, state);
}

void UIFixture::SimulateMouse(int x, int y, InputState state) noexcept
{
	DX::StepTimer timer;
	m_mockInput->SetMouseState(x, y, state);
	m_uiModule->Update(timer);
}

void UIFixture::SimulateClick(const XMINT2& startPos) noexcept
{
	SimulateClick(startPos, startPos);
}

void UIFixture::SimulateClick(const XMINT2& startPos, const XMINT2& endPos) noexcept
{
	SimulateMouse(startPos, InputState::Pressed);
	SimulateMouse(endPos, InputState::Held);
	SimulateMouse(endPos, InputState::Released);
}