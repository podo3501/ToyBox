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
	//return CreateGoogleTest(argc, argv, L"Toy Unit Test", new GlobalEnv);
	 
	//�����尡 ���ư��� ������ ������ ��� ���α׷��� ����ִ� ��찡 �־ 
	//���� �׽�Ʈ�� ������ �ϴ� ��찡 �ִ�. �׷����� ����ؼ� ���α׷��� ����� �ִ��� üũ
	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, false, L"Toy Unit Test");
	if (!mutex)
		mutex = CreateMutex(nullptr, true, L"Toy Unit Test");
	else {
		OutputDebugString(L"\nTest ����! ���α׷��� �ߺ� ���� �Ǿ����ϴ�. �ڵ� ���� �մϴ�.\n\n");
		return 0;
	}

	::testing::GTEST_FLAG(color) = "yes"; //yes : �÷� ���, no : �÷� ��� x, auto : �ڵ�
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new GlobalEnv);

	auto silent_run = false;
	if (silent_run) {
		auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
		delete listeners.Release(listeners.default_result_printer());
	}

	return RUN_ALL_TESTS();
}