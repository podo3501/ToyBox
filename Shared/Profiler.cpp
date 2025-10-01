#include "pch.h"
#include "Profiler.h"
#include <chrono>
#include <thread>

//Startup �� Shutdown�� �� �ִ� ������ tdd�Ҷ� �ӵ����� �����̴�. tdd�� ������ �׽�Ʈ�� ������ ���� �� �ֱ� �����̴�.
void TracyStartupProfiler()
{
#ifdef TRACY_ENABLE
	tracy::StartupProfiler();

	// ���� ��� �ð�(������ ������ ������)
	constexpr auto connectionTimeout = chrono::milliseconds(1000); // 1�� Ÿ�Ӿƿ�
	auto startTime = chrono::steady_clock::now();

	while (!TracyIsConnected)
	{
		this_thread::sleep_for(std::chrono::milliseconds(10));
		if (std::chrono::steady_clock::now() - startTime > connectionTimeout) // Ÿ�Ӿƿ��� �߰�
			break;
	}
#endif
}

void TracyShutdownProfiler()
{
#ifdef TRACY_ENABLE
	FrameMark; // ������ ������ ����
	this_thread::sleep_for(chrono::milliseconds(100)); // ������ ���� �ð� Ȯ��

	tracy::ShutdownProfiler(); // ���� ������ �� ���� ����
#endif
}