#include "pch.h"
#include "TextAreaT.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(TextAreaT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	MATCHER_P(ColorEq, expected, "") 
	{ 
		return DirectX::XMVector4Equal(arg, expected); 
	}

	TEST_F(TextAreaT, ProcessRender)
	{
		m_component->ChangeSize(100, 150);

		//MockTextureController::MeasureText 및 MockTextureController::GetLineSpacing 참고. 
		MockTextureRender render;
		::testing::InSequence seq;
		EXPECT_CALL(render, DrawString(0, wstring(L"테스트"), Vector2{ 0.f, 0.f }, ColorEq(Colors::Black))).Times(1);
		EXPECT_CALL(render, DrawString(0, wstring(L"빨강색"), Vector2{ 30.f, 0.f }, ColorEq(Colors::Red))).Times(1);
		EXPECT_CALL(render, DrawString(1, wstring(L"Test"), Vector2{ 0.f, 25.f }, ColorEq(Colors::Black))).Times(1);
		EXPECT_CALL(render, DrawString(1, wstring(L"Blue"), Vector2{ 40.f, 25.f }, ColorEq(Colors::Blue))).Times(1);

		m_component->ProcessRender(&render);
	}

	TEST_F(TextAreaT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"TextAreaT_WR.json"));
	}
}