#include "pch.h"
#include "CoreTraverserT.h"
#include "MockComponent.h"

class BaseTraverser
{
public:
};

namespace UserInterfaceT::UIComponentT
{
	TEST_F(CoreTraverserT, Create)
	{
		auto [component, _] = CreateMockComponent<MockComponent>();

		//BaseTraverser baseTraverser;
		//EXPECT_TRUE(baseTraverser.RenameRegion(component, "MainRegion"));
		//UITraverser::RenameRegion(component, "MainRegion");
	}
	//locator + namespace + 일반 클래스 조합으로 해서 
	//UIEx(m_main).AttachComponent(move(parentOwner));
	//이런 형태가 아니라 UITraverser::AttachComponent(m_main, move(parentOwner));
	//이렇게 한다. 그리고 using을 사용하면 UITraverser를 안써도 되니까 cpp 쪽 코드에 지저분함이
	//없어지겠지? 단점은 만들고 나면 namespace에서 연결을 해 줘야 하는 정도일듯.
}
