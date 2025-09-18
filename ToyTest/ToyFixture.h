#pragma once
//엔진을 돌리지 않고 Test를 하기 위한 Fixture.
//테스트가 엄청 많은데 그때마다 DirextXTK를 계속해서 올렸다 내린다면 그것만으로도 테스트 시간이 오래 걸린다.
//그리고 IRenderer를 쓰지 않고 IRenderer를 상속받은 MockRenderer로 해야 하는게 이론상 타당하다.

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

	void TestMockRender(int expIndex, const vector<RECT>& expectDest, const string& bindKey, UIComponent* component = nullptr);

	void MockMouseInput(int mouseX, int mouseY, bool leftButton);

protected:
	void SetUp() override;
	void TearDown() override;
	TextureResourceBinder* GetResBinder() const noexcept;

	unique_ptr<MockRenderer> m_mockRenderer;
	unique_ptr<UIModule> m_uiModule;
	DX::StepTimer m_timer;
    UIComponent* m_main;
};