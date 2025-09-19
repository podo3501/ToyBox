#include "pch.h"
#include "ToyFixture.h"
#include "IMockRenderer.h"
#include "GameConfig.h"
#include "Utils/CommonUtil.h"
#include "Utils/GeometryUtil.h"
#include "InputManager.h"
#include "TestUtility.h"
#include "Utils/StlUtil.h"
#include "UserInterface/UIModule.h"
#include "UserInterface/UIComponent/UIType.h"
#include "UserInterface/UIComponent/UILayout.h"
#include "UserInterface/TextureResourceBinder/TextureBinderHelper.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/Command/UICommandList/UICommandList.h"
#include "UserInterface/Command/TexResCommandList/TexResCommandList.h"
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

	m_main = nullptr;
	m_uiModule.reset();
	m_mockRenderer.reset();

#if defined(DEBUG) | defined(_DEBUG)
	ReportLiveObjects();
#endif
}

TextureResourceBinder* ToyFixture::GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

void ToyFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times)
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

void ToyFixture::CallMockRender(function<void(size_t, const RECT&, const RECT*, const vector<RECT>&)> testRenderFunc,
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

//TextArea�� CallMockRender
void ToyFixture::CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessUpdate(m_timer);
	component->ProcessRender(&mockRender);
}

void ToyFixture::CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	m_main->ProcessUpdate(m_timer);
	m_main->ProcessRender(&mockRender);
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

void ToyFixture::CloneTest(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(clonePanel);
}

//////////////////////////////////////////////////////////////////

void TextureSwitcherTest::FitToTextureSourceTest(const string& bindingKey)
{
	TextureSwitcher* texSwitcher = UIEx(m_main).FindComponent<TextureSwitcher*>("TextureSwitcher");
	EXPECT_TRUE(texSwitcher->FitToTextureSource());
	EXPECT_EQ(texSwitcher->GetSize(), GetSizeOfBindKey(GetResBinder(), bindingKey));
}

void TextureSwitcherTest::CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(GetResBinder(), clonePanel);
	//WriteReadTest(m_resBinder.get(), clonePanel);
}

//////////////////////////////////////////////////////////////////

bool IntegrationTest::VerifyClone(unique_ptr<UIComponent> original)
{
	if (!original) return false;
	auto resBinder = m_uiModule->GetTexResBinder();
	EXPECT_TRUE(original->BindTextureSourceInfo(resBinder, m_mockRenderer->GetTextureController()));
	auto clone = original->Clone();

	return Compare(original, clone);
}

//////////////////////////////////////////////////////////////////

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