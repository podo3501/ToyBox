#pragma once
//������ ������ �ʰ� Test�� �ϱ� ���� Fixture.
//�׽�Ʈ�� ��û ������ �׶����� DirextXTK�� ����ؼ� �÷ȴ� �����ٸ� �װ͸����ε� �׽�Ʈ �ð��� ���� �ɸ���.
//�׸��� IRenderer�� ���� �ʰ� IRenderer�� ��ӹ��� MockRenderer�� �ؾ� �ϴ°� �̷л� Ÿ���ϴ�.

#include "Mocks/MockRenderer.h"
#include "StepTimer.h"

class UIModule;
class UIComponent;
class TextureResourceBinder;

class ToyFixture : public ::testing::Test
{
public:
	ToyFixture();
	~ToyFixture();

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

class BasicTest : public ToyFixture {};
class ContainerTest : public ToyFixture {};
class TextureSwitcherTest : public ToyFixture
{
protected:
	void FitToTextureSourceTest(const string& bindingKey);
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? ToyFixture Ŭ�� �Լ��� ��ĥ �� ������ ������.
};
class ComplexTest : public ToyFixture {};
class IntegrationTest : public ToyFixture
{
protected:
	bool VerifyClone(unique_ptr<UIComponent> original);
};
class TextureResourceBinderTest : public ToyFixture {};
class TracyBenchmarkTest : public ToyFixture {};

class UICommandList;
class TexResCommandList;
class UndoRedoTest : public ToyFixture
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