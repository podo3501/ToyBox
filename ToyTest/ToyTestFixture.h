#pragma once
#include "../Toy/MainLoop.h"

class Window;
class UIComponent;
struct IRenderer;
struct IComponent;

class ToyTestFixture : public ::testing::Test
{
public:
	ToyTestFixture();
	~ToyTestFixture();

	void CallMockRender(function<void(size_t, const RECT&, const RECT*)> testRenderFunc, int times);
	void CallMockRender(UIComponent* component, function<void(size_t, const RECT&, const RECT*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
	void CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
	void CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*)> renderFunc, int times);
	void TestUpdate(int mouseX, int mouseY);

protected:
	void SetUp() override;
	void TearDown() override;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIComponent> m_panel;
};

//비주얼 스튜디오 테스트 탐색기에 계층구조 표시하기 위해서 Fixture를 상속받아서 함.
//namespace 중첩으로는 계층구조로 나타나지 않음. a::b 이런식으로 나타남. 즉, 묶음으로 테스트 하기 어려움

class BasicFunctionalityTest : public ToyTestFixture	{};
class IntegrationTest : public ToyTestFixture {};
class UndoRedoTest : public ToyTestFixture {};
class IRendererTest : public ToyTestFixture {};