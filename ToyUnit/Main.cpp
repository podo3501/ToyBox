#include "pch.h"
#include "Main.h"
#include "Shared/Framework/Initializer/GoogleTest.h"

GlobalEnv::GlobalEnv() = default;
GlobalEnv::~GlobalEnv() = default;

void GlobalEnv::SetUp()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void GlobalEnv::TearDown()
{
	//Dx 메모리릭을 테스트 하지 않는 이유는 Dx를 안띄우기 때문이다.
}

//GlobalEnv 클래스의 SetUp을 해주기 위해서.
int main(int argc, char** argv)
{
	return CreateGoogleTest(argc, argv, L"Toy Unit Test", new GlobalEnv);
}