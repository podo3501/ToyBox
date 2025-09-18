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

	MockMouseInput(-1, -1, false); //Ű����, ���콺�� stataic Ŭ���� �̱� ������ �����͸� �ʱ�ȭ ��Ų��.
	//�޸� �� ���� �����ش�. ������ ����� ������ ���� ������ �ʾƼ� �޸𸮰� ���� �ִµ�
	//ReportLiveObjects �Լ��� �޸𸮰� �� ���� ���ٰ� �޼����� ���� �����̴�.

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
{ //���콺�� ���°��� ������Ʈ�� ����ص� ������ ���°��� ��� ����.
	auto& mouseTracker = const_cast<MouseTracker&>(InputManager::GetMouse());
	auto state = mouseTracker.GetLastState();
	state.x = mouseX;
	state.y = mouseY;
	state.leftButton = leftButton;
	mouseTracker.Update(state);
}
