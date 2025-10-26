#include "pch.h"
#include "TextAreaT.h"
#include "Shared/SerializerIO/SerializerIO.h"

namespace UserInterface::UIComponentT::ComponentT
{
	TEST_F(TextAreaT, Clone)
	{
		auto clone = m_component->Clone();
		EXPECT_TRUE(CompareDerived(m_component, clone));
	}

	TEST_F(TextAreaT, ProcessRender)
	{
		m_component->ChangeSize(100, 150);

		MockTextureRender render;
		EXPECT_CALL(render, DrawString(::testing::_, ::testing::_, ::testing::_, ::testing::_))
			.WillRepeatedly([](size_t index, const std::wstring& text, const Vector2& pos, FXMVECTOR color) {
				int a = 1;
				});

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(TextAreaT, WriteAndRead)
	{
		wstring filename = L"../Resources/Test/Temp/TextAreaT_WriteAndRead.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_component, filename));

		unique_ptr<TextArea> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));

		EXPECT_TRUE(CompareDerived(m_component, read));
	}
}