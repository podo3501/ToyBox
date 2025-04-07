#pragma once
#include "../Toy/MainLoop.h"

class Window;
class UIComponent;
class TextureResourceBinder;
struct IRenderer;
struct IComponent;

class ToyTestFixture : public ::testing::Test
{
public:
	ToyTestFixture();
	~ToyTestFixture();

	void CallMockRender(UIComponent* component, function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
	void CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);

	void CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> renderFunc, int times);
	void MockMouseInput(int mouseX, int mouseY, bool leftButton = false);

protected:
	void SetUp() override;
	void TearDown() override;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIComponent> m_panel;
	unique_ptr<TextureResourceBinder> m_resBinder;
};

//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

class IRendererTest : public ToyTestFixture {};
class BasicComponentTest : public ToyTestFixture {};
class ImageSwitcherComponentTest : public ToyTestFixture 
{
public:
	void FitToTextureSourceTest(const string& bindingKey);
};
class ComplexComponentTest : public ToyTestFixture {};

class UICommandList;
class TexResCommandList;
class IntegrationTest : public ToyTestFixture 
{
protected:
	bool VerifyClone(unique_ptr<UIComponent> original);
};
class TextureResourceBinderTest : public ToyTestFixture {};
class UndoRedoTest : public ToyTestFixture
{
protected:
	void CaptureSnapshot(vector<unique_ptr<UIComponent>>& history);
	void CaptureSnapshot(vector<unique_ptr<TextureResourceBinder>>& history);
	void VerifyUndoRedo(UICommandList& cmdList, const vector<unique_ptr<UIComponent>>& history);
	void VerifyUndoRedo(TexResCommandList& cmdList, const vector<unique_ptr<TextureResourceBinder>>& history);

	template <typename History, typename Func>
	void ExecuteAndCapture(History& history, Func&& func)
	{
		func();
		CaptureSnapshot(history);
	}
};