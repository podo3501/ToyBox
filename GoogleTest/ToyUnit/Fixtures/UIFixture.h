#pragma once
//엔진을 돌리지 않고 Test를 하기 위한 Fixture.
//테스트가 엄청 많은데 그때마다 DirextXTK를 계속해서 올렸다 내린다면 그것만으로도 테스트 시간이 오래 걸린다.
//그리고 IRenderer를 쓰지 않고 IRenderer를 상속받은 MockRenderer로 해야 하는게 이론상 타당하다.

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
