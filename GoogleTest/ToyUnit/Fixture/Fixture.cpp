#include "pch.h"
#include "Fixture.h"
#include "UserInterface/Helper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/System/Input.h"
#include "Shared/Framework/Environment.h"

using ::testing::_;
using ::testing::Invoke;

Fixture::~Fixture() = default;
Fixture::Fixture() :
	m_main{ nullptr }
{}

void Fixture::SetUp()
{
	Rectangle rc{ 0, 0, 800, 600 };
	UILayout layout{ GetSizeFromRectangle(rc), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_mockRenderer = make_unique<MockRenderer>();
	m_uiModule = CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename);
	m_main = m_uiModule->GetMainPanel();
}

void Fixture::TearDown()
{
	MockMouseInput(-1, -1, false); //키보드, 마우스는 stataic 클래스 이기 때문에 데이터를 초기화 시킨다.
}

TextureResourceBinder* Fixture::GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

void Fixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times)
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

void Fixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, const vector<RECT>&)> testRenderFunc,
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

void Fixture::TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component)
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
void Fixture::CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessUpdate(m_timer);
	component->ProcessRender(&mockTexRender);
}

void Fixture::CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockTextureRender mockTexRender;
	EXPECT_CALL(mockTexRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockTexRender);
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

void Fixture::CloneTest(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(clonePanel);
}