#pragma once
//������ ������ �ʰ� Test�� �ϱ� ���� Fixture.
//�׽�Ʈ�� ��û ������ �׶����� DirextXTK�� ����ؼ� �÷ȴ� �����ٸ� �װ͸����ε� �׽�Ʈ �ð��� ���� �ɸ���.
//�׸��� IRenderer�� ���� �ʰ� IRenderer�� ��ӹ��� MockRenderer�� �ؾ� �ϴ°� �̷л� Ÿ���ϴ�.

#include "Mocks/MockRenderer.h"
#include "Shared/System/StepTimer.h"

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
