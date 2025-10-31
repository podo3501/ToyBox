#include "pch.h"
#include "UIModuleAsComponentT.h"

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(UIModuleAsComponentT, GetUIModule)
	{
		EXPECT_TRUE(m_component->GetUIModule());
	}
}