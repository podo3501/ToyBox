#include "pch.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/StlTypeExt.hpp"
#include "Shared/Utils/StlExt.h"
#include "Shared/SerializerIO/Storage/JsonStorageLocator.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

namespace Utility
{
	TEST(Client, TextAreaParser)
	{
		TextProperty textProperty;
		auto result = Parser(
			L"<Hangle><Red>Å×½ºÆ®, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>",
			textProperty);
		EXPECT_TRUE(result);
	}
}
