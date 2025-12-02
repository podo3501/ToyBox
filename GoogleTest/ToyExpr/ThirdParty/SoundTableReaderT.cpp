#include "pch.h"
#include "SoundTableReaderT.h"

namespace ThirdParty
{
	TEST_F(SoundTableReaderT, GetInfo)
	{
		m_reader.Read(L"Sound/SoundTable.json");
		auto info = m_reader.GetInfo("TestWav_01");

		EXPECT_EQ(info->filename, "Sound/Wav/test1.wav");
	}

	TEST_F(SoundTableReaderT, Read)
	{
		EXPECT_TRUE(m_reader.Read(L"Sound/SoundTable.json"));
	}
}