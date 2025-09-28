#pragma once
//엔진을 돌리지 않고 Test를 하기 위한 Fixture.
//테스트가 엄청 많은데 그때마다 DirextXTK를 계속해서 올렸다 내린다면 그것만으로도 테스트 시간이 오래 걸린다.
//그리고 IRenderer를 쓰지 않고 IRenderer를 상속받은 MockRenderer로 해야 하는게 이론상 타당하다.

#include "Mocks/MockRenderer.h"
#include "StepTimer.h"

class UIModule;
class UIComponent;
class TextureResourceBinder;

class UIFixture : public ::testing::Test
{
public:
	UIFixture();
	~UIFixture();

	void CallMockRender(function<void(size_t, const RECT&, const RECT*, TextureResourceBinder*)> testRenderFunc, int times);
	void CallMockRender(function<void(size_t, const RECT&, const RECT*, const vector<RECT>&)> testRenderFunc,
		const string& bindKey, int times);
	void TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component = nullptr);

	//TextArea
	void CallMockRender(function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
	void CallMockRender(UIComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);

	void CloneTest(const vector<RECT>& expectDest, const string& bindKey);
	void MockMouseInput(int mouseX, int mouseY, bool leftButton = false);

protected:
	void SetUp() override;
	void TearDown() override;
	TextureResourceBinder* GetResBinder() const noexcept;

	unique_ptr<MockRenderer> m_mockRenderer;
	unique_ptr<UIModule> m_uiModule;
	DX::StepTimer m_timer;
    UIComponent* m_main;
};

//비주얼 스튜디오 테스트 탐색기에 계층구조 표시하기 위해서 Fixture를 상속받아서 함.
//namespace 중첩으로는 계층구조로 나타나지 않음. a::b 이런식으로 나타남. 즉, 묶음으로 테스트 하기 어려움

class BasicComponent : public UIFixture {};
class ComplexComponent : public UIFixture {};
class ContainerComponent : public UIFixture {};
class Integration : public UIFixture
{
protected:
	bool VerifyClone(unique_ptr<UIComponent> original);
};
class TextureResourceBinderTest : public UIFixture {};
class TextureSwitcherComponent : public UIFixture
{
protected:
	void FitToTextureSourceTest(const string& bindingKey);
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? ToyFixture 클론 함수와 합칠 수 있을꺼 같은데.
};
class TracyBenchmark : public UIFixture {};

class UICommandList;
class TexResCommandList;
class UIModuleConnection : public UIFixture {};
class UndoRedo : public UIFixture
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