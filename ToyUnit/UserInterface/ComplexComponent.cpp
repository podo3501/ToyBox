#include "pch.h"
#include "UIFixture.h"
#include "Helper.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/UIComponent/Components/ScrollBar.h"
#include "UserInterface/UIComponent/Components/RenderTexture.h"
#include "UserInterface/UIComponent/Components/ListArea.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "InputManager.h"

namespace UserInterface
{
	TEST_F(ComplexComponent, ListArea)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UIEx(m_main).AttachComponent(move(listArea), { 400, 300 });
		EXPECT_TRUE(m_uiModule->BindTextureResources());
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, listAreaPtr);

		EXPECT_TRUE((listAreaPtr->GetSize() == XMUINT2{ 48, 48 }));
		listAreaPtr->ChangeSize({ 150, 128 });
		listAreaPtr->ChangeOrigin(Origin::Center);

		auto scrollBarPtr = UIEx(listAreaPtr).FindComponent<ScrollBar*>("ScrollBar");
		EXPECT_TRUE(UIEx(m_main).IsPositionUpdated());

		EXPECT_TRUE(MakeSampleListAreaData(m_mockRenderer.get(), GetResBinder(), listAreaPtr, 5));
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));
		auto preSizeX = listAreaPtr->GetContainer(0)->GetSize().x;

		listAreaPtr->RemoveContainer(0);
		EXPECT_FALSE(scrollBarPtr->HasStateFlag(StateFlag::Render));
		//auto curSizeX = listAreaPtr->GetContainer(0)->GetSize().x;
		//EXPECT_NE(preSizeX, curSizeX);

		//auto renderTexturePtr = UIEx(listAreaPtr).FindComponent<RenderTexture*>("RenderTexture");
		//EXPECT_TRUE(listAreaPtr->ChangeSize({ 150, 64 }));
		//EXPECT_EQ(renderTexturePtr->GetSize(), XMUINT2(150, 64));
		//EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Render));

		//auto scrollContainerPtr = UIEx(listAreaPtr).FindComponent<TextureSwitcher*>("TextureSwitcher");
		//EXPECT_EQ(scrollContainerPtr->GetSize().y, 30); //��ũ�� ���ηε� ��ư ����. (64 - padding) * (60 / 120) �� scrollBar ����(-padding)�� ������ ������ ����ؼ� ũ�������� 

		//listAreaPtr->ClearContainers();
		//EXPECT_FALSE(listAreaPtr->RemoveContainer(0));
	}
	
	TEST_F(ComplexComponent, ListAreaToolMode)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UIEx(m_main).AttachComponent(move(listArea), { 400, 300 });
		m_uiModule->BindTextureResources();

		//save�� �Ű澵 �ʿ� ����. ����嵵 ���̺� ���� �ʴ´�. �ϴ� ����� ������ ����.
		listAreaPtr->EnableToolMode(true); //false���� true�� �ٲ�� ������ entered�� ȣ��ȴ�.

		//����尡 �۵��ϰ� �ִ��� Ȯ���Ѵ�. �⺻�� �������� �ʴ� ���̴�. ����尡 �ִ� ���ٰ� �ƴ϶� ������ΰ� ������ ���谡 ���� �����ϴ� ���̴�.
		auto protoContainer = listAreaPtr->GetPrototypeContainer();

		//��ü ������Ʈ�� ����忡�� �����Ѵ�.
		//UIEx(m_panel)->EnableToolMode(false); //true���� false�� �ٲ�� ������ leaved�� ȣ��ȴ�.

		//����尡 �ƴѰ��� Ȯ���Ѵ�.
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source, const vector<RECT>&)
	{
		EXPECT_EQ(index, 5); //0, 1�� ��Ʈ. 2, 3�� texture. 4�� ó�� renderTexture, 5�� WriteRead�� renderTexture
		vector<RECT> expectDest = { { 75, 75, 125, 125 } };
		vector<RECT> expectSource = { { 0, 0, 50, 50 } };
		TestCoordinates(2, dest, source, 2, expectDest, expectSource);//�� ���ϴϱ� 2�� �׳� �־���. 
	}

	TEST_F(ComplexComponent, RenderTexture)
	{
		auto switcher = CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal);
		auto [renderTex, renderTexPtr] = GetPtrs(CreateComponent<RenderTexture>(move(switcher)));
		UIEx(m_main).AttachComponent(move(renderTex), { 100, 100 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, renderTexPtr);

		EXPECT_TRUE((renderTexPtr->GetSize() == XMUINT2{ 32, 32 }));
		renderTexPtr->ChangeSize({ 50, 50 });
		renderTexPtr->ChangeOrigin(Origin::Center);

		CallMockRender(TestRenderTexture, "", 1); //core�� �����ڵ尡 �� ���� ������ �ѹ��� ���´�.
	}

	////////////////////////////////////////////////////////

	static void TestScrollBar(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { 
			{ 92, 100, 108, 110 }, { 92, 110, 108, 290 }, { 92, 290, 108, 300 },
			{ 92, 175, 108, 182 }, { 92, 182, 108, 218 }, { 92, 218, 108, 225 } };

		vector<RECT> expectSource{ GetSources(rb, "ScrollTrack3_V") };
		vector<RECT> expectSource2{ GetSources(rb, "ScrollButton3_V_Normal") };
		expectSource.insert(expectSource.end(), expectSource2.begin(), expectSource2.end());
		TestCoordinates(index, dest, source, 2, expectDest, expectSource);
	}

	TEST_F(ComplexComponent, ScrollBar)
	{
		auto [scrollBar, scrollBarPtr] = GetPtrs(CreateSampleScrollBar({}, DirectionType::Vertical));
		UIEx(m_main).AttachComponent(move(scrollBar), { 100, 200 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, scrollBarPtr);

		EXPECT_TRUE((scrollBarPtr->GetSize() == XMUINT2{ 16, 48 }));
		scrollBarPtr->ChangeSize({ 16, 200 });
		scrollBarPtr->ChangeOrigin(Origin::Center);

		testing::MockFunction<void(float)> mockOnScrollChanged;
		scrollBarPtr->AddScrollChangedCB(mockOnScrollChanged.AsStdFunction());

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollBarPtr->UpdateScrollView(viewArea, contentSize);
		scrollBarPtr->SetPositionRatio(0.5f);
		EXPECT_TRUE(scrollBarPtr->HasStateFlag(StateFlag::Active));
		CallMockRender(TestScrollBar, 6);

		EXPECT_CALL(mockOnScrollChanged, Call(testing::FloatEq(85.f / 150.f))).Times(1);

		MockMouseInput(100, 200, true); //Pressed
		m_main->ProcessUpdate(m_timer);

		MockMouseInput(110, 210, true); //������� Pressed�� �Ǿ���Ѵ�.
		m_main->ProcessUpdate(m_timer);
	}

	////////////////////////////////////////////////////////

	TEST_F(ComplexComponent, Switcher_Scroll)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(UILayout{ {16, 100}, Origin::Center },
			TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::HoldToKeepPressed));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, switcherPtr);

		testing::MockFunction<void(KeyState)> mockOnPress;
		switcherPtr->AddPressCB(mockOnPress.AsStdFunction());

		EXPECT_CALL(mockOnPress, Call(KeyState::Pressed)).Times(1); //Pressed ���ڸ� �־ �ѹ� ȣ���� ���� ���
		EXPECT_CALL(mockOnPress, Call(KeyState::Held)).Times(1);

		MockMouseInput(100, 100, true); //Pressed
		vector<RECT> exDest = { { 92, 50, 108, 57 }, { 92, 57, 108, 143 }, { 92, 143, 108, 150 } };
		TestMockRender(2, exDest, "ScrollButton3_V_Pressed");

		MockMouseInput(110, 110, true); //�������� ������� holdToKeepPressed �ɼ��� �ֱ� ������ Pressed�� �Ǿ���Ѵ�.
		TestMockRender(2, exDest, "ScrollButton3_V_Pressed");
	}
}