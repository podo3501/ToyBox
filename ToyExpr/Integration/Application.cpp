#include "pch.h"
#include "IRenderer.h"
#include "Toy/GameLoop.h"
#include "Shared/Framework/Initializer/Application.h"

namespace Integration
{
	//������ �����ؼ� �������� ������ Ȯ���Ѵ�.
	//�� �׽�Ʈ�� ������ ����� �ϴ� ���̱� ������ ��ҿ��� �� �׽�Ʈ�� ���� �ʴ´�.
	//��ҿ��� DISABLED�� �����ϰ� �ѹ��� ������ ����� �׽�Ʈ �ؾ� �� ��쿡�� DISABLED_�� �����Ѵ�.
	TEST(Application, MultipleAppExcute)
	{
		for (auto i : std::views::iota(0, 5))
		{
			RECT windowRect = { 0, 0, 800, 600 };
			std::wstring resourcePath = L"Resources/";
			auto gameLoop = CreateAppLoop<GameLoop>(GetModuleHandle(nullptr), SW_HIDE, windowRect, resourcePath, false);
			EXPECT_TRUE(gameLoop != nullptr);

			gameLoop.reset();
		}
	}
}
