#include "pch.h"
#include "IRenderer.h"
#include "Toy/GameLoop.h"
#include "Shared/Framework/Initializer/Application.h"

namespace Integration
{
	//여러번 실행해서 오동작이 나는지 확인한다.
	//이 테스트는 엔진을 띄워서 하는 것이기 때문에 평소에는 이 테스트를 켜지 않는다.
	//평소에는 DISABLED로 설정하고 한번씩 엔진을 띄워서 테스트 해야 할 경우에는 DISABLED_를 삭제한다.
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
