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
	//Dx �޸𸮸��� �׽�Ʈ ���� �ʴ� ������ Dx�� �ȶ��� �����̴�.
}

//GlobalEnv Ŭ������ SetUp�� ���ֱ� ���ؼ�.
int main(int argc, char** argv)
{
	return CreateGoogleTest(argc, argv, L"Toy Unit Test", new GlobalEnv);
}