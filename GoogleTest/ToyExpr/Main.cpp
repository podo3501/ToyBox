#include "pch.h"
#include "Main.h"
#include "Shared/Framework/Initializer/GoogleTest.h"
#include "Core/Utils/DxLeakCheck.h"

GlobalEnv::GlobalEnv() = default;
GlobalEnv::~GlobalEnv() = default;

void GlobalEnv::SetUp()
{
#ifdef DISABLE_ToyExpr
	exit(0);
#endif

	//�޸� ���� ��� ���� �ɼ�
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void GlobalEnv::TearDown()
{
#if defined(DEBUG) | defined(_DEBUG)
	CheckDirectxLeaks();
#endif
}

//GlobalEnv Ŭ������ SetUp�� ���ֱ� ���ؼ�.
int main(int argc, char** argv)
{
	return CreateGoogleTest(argc, argv, L"Toy Experiment", new GlobalEnv);
}