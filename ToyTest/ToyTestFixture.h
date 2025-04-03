#pragma once
#include "../Toy/MainLoop.h"

class Window;
class UIComponent;
class TextureSourceBinder;
struct IRenderer;
struct IComponent;

class ToyTestFixture : public ::testing::Test
{
public:
	ToyTestFixture();
	~ToyTestFixture();

	void CallMockRender(UIComponent* component, function<void(size_t, const RECT&, const RECT*, TextureSourceBinder*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureSourceBinder*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
	void CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);

	void CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*, TextureSourceBinder*)> renderFunc, int times);
	void MockMouseInput(int mouseX, int mouseY, bool leftButton = false);

protected:
	void SetUp() override;
	void TearDown() override;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIComponent> m_panel;
	unique_ptr<TextureSourceBinder> m_sourceBinder;
};

//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

class IRendererTest : public ToyTestFixture {};
class BasicComponentTest : public ToyTestFixture {};
class ComplexComponentTest : public ToyTestFixture {};

class UICommandList;
class TexSrcCommandList;
class IntegrationTest : public ToyTestFixture 
{
protected:
	bool VerifyClone(unique_ptr<UIComponent> original);
};
class TextureSourceBinderTest : public ToyTestFixture {};
class UndoRedoTest : public ToyTestFixture
{
protected:
	void CaptureSnapshot(UICommandList& cmdList, vector<unique_ptr<UIComponent>>& history);
	void CaptureSnapshot(TexSrcCommandList& cmdList, vector<unique_ptr<TextureSourceBinder>>& history);
	void VerifyUndoRedo(UICommandList& cmdList, const vector<unique_ptr<UIComponent>>& history);
	void VerifyUndoRedo(TexSrcCommandList& cmdList, const vector<unique_ptr<TextureSourceBinder>>& history);

	template <typename cmdListType, typename History, typename Func>
	void ExecuteAndCapture(cmdListType& cmdList, History& history, Func&& func)
	{
		func();
		CaptureSnapshot(cmdList, history);
	}
};