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