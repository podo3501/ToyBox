#include "pch.h"
#include "../Toy/MainLoop.h"

//������ �����ؼ� �������� ������ Ȯ���Ѵ�.
TEST(MainLoop, MultipleInitializeTest)
{
	for(int i = 1; i < 5; ++i)
	{
		MainLoop mainLoop;
		EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_HIDE));
	}
}

TEST(MainLoop, RunTest)
{
	MainLoop mainLoop;
	EXPECT_TRUE(mainLoop.Initialize(GetModuleHandle(nullptr), L"Resources/", SW_SHOWDEFAULT));
	EXPECT_EQ(mainLoop.Run(), 0);
}