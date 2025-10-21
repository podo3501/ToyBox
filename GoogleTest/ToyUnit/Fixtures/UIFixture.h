#pragma once
//������ ������ �ʰ� Test�� �ϱ� ���� Fixture.
//�׽�Ʈ�� ��û ������ �׶����� DirextXTK�� ����ؼ� �÷ȴ� �����ٸ� �װ͸����ε� �׽�Ʈ �ð��� ���� �ɸ���.
//�׸��� IRenderer�� ���� �ʰ� IRenderer�� ��ӹ��� MockRenderer�� �ؾ� �ϴ°� �̷л� Ÿ���ϴ�.

#include "Mocks/MockRenderer.h"
#include "Shared/System/StepTimer.h"

class UIModule;
class UIComponent;
class MockInputManager;
class TextureResourceBinder;
enum class InputState;

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

protected:
	virtual void SetUp() override;
	virtual void TearDown() override;
	TextureResourceBinder* GetResBinder() const noexcept;

	void SimulateMouse(const XMINT2& pos, InputState state) noexcept;
	void SimulateMouse(int x, int y, InputState state) noexcept;
	void SimulateClick(const XMINT2& startPos) noexcept;
	void SimulateClick(const XMINT2& startPos, const XMINT2& endPos) noexcept;

	unique_ptr<MockRenderer> m_mockRenderer;
	unique_ptr<MockInputManager> m_mockInput;
	unique_ptr<UIModule> m_uiModule;
	DX::StepTimer m_timer;
    UIComponent* m_main;
};
