#include "pch.h"
#include "../Toy/MainLoop.h"

//여러번 실행해서 오동작이 나는지 확인한다.
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