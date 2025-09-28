#pragma once
//������ ������ �ʰ� Test�� �ϱ� ���� Fixture.
//�׽�Ʈ�� ��û ������ �׶����� DirextXTK�� ����ؼ� �÷ȴ� �����ٸ� �װ͸����ε� �׽�Ʈ �ð��� ���� �ɸ���.
//�׸��� IRenderer�� ���� �ʰ� IRenderer�� ��ӹ��� MockRenderer�� �ؾ� �ϴ°� �̷л� Ÿ���ϴ�.

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

//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

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
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? ToyFixture Ŭ�� �Լ��� ��ĥ �� ������ ������.
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