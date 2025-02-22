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

//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

class BasicFunctionalityTest : public ToyTestFixture	{};
class IntegrationTest : public ToyTestFixture {};
class UndoRedoTest : public ToyTestFixture {};
class IRendererTest : public ToyTestFixture {};