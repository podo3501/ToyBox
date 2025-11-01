#include "pch.h"
#include "UIModuleT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"
#include "Shared/System/StepTimer.h"
#include "Shared/Utils/StlExt.h"

class MockClickable : public UIComponentStub
{
public:
	MockClickable()
	{
		//������ ���� ������ UpdateMouseState �Լ� �ȿ��� ������Ʈ�� capture ���� �ʾ� �Լ��� �������� ����.
		ON_CALL(*this, OnPress()).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnHold(testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnRelease(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, OnNormal, (), (noexcept));
	MOCK_METHOD(bool, OnHover, (), (noexcept));
	MOCK_METHOD(bool, OnPress, (), (noexcept));
	MOCK_METHOD(bool, OnHold, (bool inside), (noexcept));
	MOCK_METHOD(bool, OnRelease, (bool inside), (noexcept));
};

class MockToolMode : public UIComponentStub
{
public:
	MOCK_METHOD(bool, EnterToolMode, (), (noexcept));
	MOCK_METHOD(bool, ExitToolMode, (), (noexcept));
};

class MockBehaviorComponent : public UIComponentStub
{
public:
	MockBehaviorComponent()
	{
		//�⺻ ������ false �̱� ������ ������ ���� ������ ����ε� ������ ���Ѵٰ� �����Ѵ�.
		ON_CALL(*this, ImplementBindSourceInfo(testing::_, testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, ImplementUpdate(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, ImplementBindSourceInfo, (TextureResourceBinder*, ITextureController*), (noexcept)); //������ �޸𸮿��� �ҷ��ͼ� Ű���� ��Ī��Ű�� �Լ�
	MOCK_METHOD(bool, ImplementUpdate, (const DX::StepTimer&), (noexcept)); //������Ʈ �ϸ鼭 ��ǥ�� ���
	MOCK_METHOD(void, ImplementRender, (ITextureRender*), (const)); //ȭ�鿡 �����ִ� �κ�
};

/////////////////////////////////////////////////////////

namespace UserInterfaceT
{
	TEST_F(UIModuleT, Clone)
	{
		auto clone = m_uiModule->Clone();
		EXPECT_TRUE(Compare(m_uiModule, clone));
	}

	TEST_F(UIModuleT, EnableToolMode)
	{
		auto comp = CreateOneLevelComponent<MockToolMode>(m_main);

		//�� ��带 ȣ���ϸ� ��� ������Ʈ�� ToolMode �Լ��� ȣ��
		EXPECT_CALL(*comp, EnterToolMode()).Times(1);
		m_uiModule->EnableToolMode(true); 

		EXPECT_CALL(*comp, ExitToolMode()).Times(1);
		m_uiModule->EnableToolMode(false);
	}

	TEST_F(UIModuleT, RenderPipeline)
	{
		auto component = CreateOneLevelComponent<MockBehaviorComponent>(m_main);
		testing::InSequence seq;
		EXPECT_CALL(*component, ImplementBindSourceInfo(testing::_, testing::_)).Times(1);
		EXPECT_CALL(*component, ImplementUpdate(testing::_)).Times(1);
		EXPECT_CALL(*component, ImplementRender(testing::_)).Times(1);

		m_uiModule->BindTextureResources();

		DX::StepTimer timer{};
		m_uiModule->Update(timer);

		MockTextureRender render;
		m_uiModule->Render(&render);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseClickedInside)
	{
		auto comp = CreateOneLevelComponent<MockClickable>(m_main);

		EXPECT_CALL(*comp, OnHover()).Times(3); //UpdateMouseState �Ҹ������� ȣ��
		testing::InSequence seq;
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnHold(true)).Times(1);
		EXPECT_CALL(*comp, OnRelease(true)).Times(1);

		SimulateClick(comp->GetLeftTop());
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseClickedOutside)
	{
		XMINT2 outsidePos{ 0, 0 };
		auto comp = CreateOneLevelComponent<MockClickable>(m_main);

		testing::InSequence seq; //ȣ����� ����
		EXPECT_CALL(*comp, OnHover()).Times(1); //�ȿ� �������� �Ҹ��� ������ �ѹ�
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnNormal()).Times(1); //���� ������ �����鼭 ȣ���� �ѹ� �Ѵ�.
		EXPECT_CALL(*comp, OnHold(false)).Times(1);
		EXPECT_CALL(*comp, OnRelease(false)).Times(1);

		SimulateClick(comp->GetLeftTop(), outsidePos);
	}

	TEST_F(UIModuleT, UpdateMouseState_MouseHover)
	{
		ComponentDesc desc{ 10, 10, UILayout({ 50, 50 }) };
		auto parent = CreateOneLevelComponent<MockClickable>(m_main, desc);		
		auto child = CreateOneLevelComponent<MockClickable>(parent, desc);

		//���콺�� �ø��� hover �Ǵ��� Ȯ��.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnHover()).Times(1);

		SimulateMouse(child->GetLeftTop(), InputState::Up);

		//parent�� �̵��ϸ� child�� OnNormal�� ȣ��ǰ� parent�� OnHover�� ȣ��Ǿ�� ��.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnNormal()).Times(1);

		SimulateMouse(parent->GetLeftTop(), InputState::Up);
	}

	TEST_F(UIModuleT, WriteAndRead)
	{
		const wstring filename = GetTempDir() + L"UIModuleT_WR.json";
		EXPECT_TRUE(m_uiModule->Write(filename));

		auto read = CreateUIModule(filename, m_renderer.get());
		EXPECT_TRUE(read->BindTextureResources());

		EXPECT_TRUE(Compare(m_uiModule, read));
	}
}