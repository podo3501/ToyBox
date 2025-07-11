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
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? 이 두 클론 함수 합칠 수 있을꺼 같은데.
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

//비주얼 스튜디오 테스트 탐색기에 계층구조 표시하기 위해서 Fixture를 상속받아서 함.
//namespace 중첩으로는 계층구조로 나타나지 않음. a::b 이런식으로 나타남. 즉, 묶음으로 테스트 하기 어려움

class IRendererTest : public ToyTestFixture {};
class BasicComponentTest : public ToyTestFixture {};
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