#include "pch.h"
#include "GoogleTest.h"

int CreateGoogleTest(int argc, char** argv, const wstring& projectName, ::testing::Environment* env, bool enable)
{
	//�����尡 ���ư��� ������ ������ ��� ���α׷��� ����ִ� ��찡 �־ 
	//���� �׽�Ʈ�� ������ �ϴ� ��찡 �ִ�. �׷����� ����ؼ� ���α׷��� ����� �ִ��� üũ
	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, false, projectName.c_str());
	if (!mutex)
		mutex = CreateMutex(nullptr, true, projectName.c_str());
	else {
		OutputDebugString(L"\nTest ����! ���α׷��� �ߺ� ���� �Ǿ����ϴ�. �ڵ� ���� �մϴ�.\n\n");
		return 0;
	}

	::testing::GTEST_FLAG(color) = "yes"; //yes : �÷� ���, no : �÷� ��� x, auto : �ڵ�
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(env);

	auto silent_run = false;
	if (silent_run) {
		auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
		delete listeners.Release(listeners.default_result_printer());
	}

	return RUN_ALL_TESTS();
}