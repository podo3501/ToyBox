#include "pch.h"
#include "../ToyFixture.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/UIComponent/Components/ListArea.h"

namespace UserInterfaceTest
{
	TEST_F(TracyBenchmarkTest, SimpleTest)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UIEx(m_main).AttachComponent(move(listArea), { 400, 300 });
		MakeSampleListAreaData(m_mockRenderer.get(), GetResBinder(), listAreaPtr, 20);
	}
}