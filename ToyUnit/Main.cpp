#include "pch.h"
#include "Main.h"

GlobalEnv::GlobalEnv() = default;
GlobalEnv::~GlobalEnv() = default;

void GlobalEnv::SetUp()
{
	//�޸� ���� ��� ���� �ɼ�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void GlobalEnv::TearDown()
{}

//GlobalEnv Ŭ������ SetUp�� ���ֱ� ���ؼ�.
int main(int argc, char** argv)
{
	//�����尡 ���ư��� ������ ������ ��� ���α׷��� ����ִ� ��찡 �־ 
	//���� �׽�Ʈ�� ������ �ϴ� ��찡 �ִ�. �׷����� ����ؼ� ���α׷��� ����� �ִ��� üũ
	HANDLE Mutex;
	std::wstring progMutex{ L"Toy Unit Test" }; // ������Ʈ��
	if ((Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, progMutex.c_str())) == NULL)
		Mutex = CreateMutex(NULL, true, progMutex.c_str());
	else
	{
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