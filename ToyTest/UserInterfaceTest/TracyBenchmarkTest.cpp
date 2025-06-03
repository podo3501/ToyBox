#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "../Toy/UserInterface/UIModule.h"

namespace UserInterfaceTest
{
	TEST_F(TracyBenchmarkTest, SimpleTest)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		m_uiModule->AttachComponent(m_main, move(listArea), { 400, 300 });
		MakeSampleListAreaData(m_renderer.get(), GetResBinder(), listAreaPtr, 1);
	}
}