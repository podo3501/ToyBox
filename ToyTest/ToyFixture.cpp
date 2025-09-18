#include "pch.h"
#include "ToyFixture.h"
#include "IMockRenderer.h"
#include "GameConfig.h"
#include "Utils/CommonUtil.h"
#include "Utils/GeometryUtil.h"
#include "InputManager.h"
#include "TestUtility.h"
#include "UserInterface/UIModule.h"
#include "UserInterface/UIComponent/UIType.h"
#include "UserInterface/UIComponent/UILayout.h"
#include "TestHelper.h"

using ::testing::_;
using ::testing::Invoke;

ToyFixture::~ToyFixture() = default;
ToyFixture::ToyFixture() :
	m_main{ nullptr }
{}

void ToyFixture::SetUp()
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

void ToyFixture::TearDown()
{
	TracyShutdownProfiler();

	MockMouseInput(-1, -1, false); //키보드, 마우스는 stataic 클래스 이기 때문에 데이터를 초기화 시킨다.
	//메모리 안 새게 지워준다. 강제로 지우는 이유는 아직 끝나지 않아서 메모리가 남아 있는데
	//ReportLiveObjects 함수가 메모리가 안 지워 졌다고 메세지를 띄우기 때문이다.

	m_uiModule.reset();
	m_mockRenderer.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

TextureResourceBinder* ToyFixture::GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

void ToyFixture::TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component)
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

void ToyFixture::MockMouseInput(int mouseX, int mouseY, bool leftButton)
{ //마우스의 상태값은 업데이트를 계속해도 셋팅한 상태값이 계속 들어간다.
	auto& mouseTracker = const_cast<MouseTracker&>(InputManager::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	state.leftButton = leftButton;
	mouseTracker.Update(state);
}
