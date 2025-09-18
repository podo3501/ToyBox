#pragma once
#include "../Toy/MainLoop.h"

class Window;
class UIComponent;
class UIModule;
class TextureResourceBinder;
struct IRenderer;
struct IComponent;

class ToyTestFixture : public ::testing::Test
{
public:
	ToyTestFixture();
	~ToyTestFixture();

	void CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const RECT&, const RECT*, const vector<RECT>&)> testRenderFunc,
		const string& bindKey, int times);
	void TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component = nullptr);

	//TextArea
	void CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
	void CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);

	void CloneTest(const vector<RECT>& expectDest, const string& bindKey);
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? �� �� Ŭ�� �Լ� ��ĥ �� ������ ������.
	void MockMouseInput(int mouseX, int mouseY, bool leftButton = false);

protected:
	void SetUp() override;
	void TearDown() override;
	TextureResourceBinder* GetResBinder() const noexcept;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIModule> m_uiModule;
	UIComponent* m_main;
};

//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

class IRendererTest : public ToyTestFixture {};
class ContainerComponentTest : public ToyTestFixture {};
class TextureSwitcherComponentTest : public ToyTestFixture 
{
protected:
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
class TracyBenchmarkTest : public ToyTestFixture {};
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