#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/UIComponent/Components/ListArea.h"

namespace Integration
{
	TEST_F(TracyBenchmark, MakeListAreaData)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UIEx(m_main).AttachComponent(move(listArea), { 400, 300 });
		MakeSampleListAreaData(m_renderer.get(), GetResBinder(), listAreaPtr, 20);
	}
}