#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"

namespace UserInterfaceTest
{
	TEST_F(TracyBenchmarkTest, SimpleTest)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		MakeSampleListAreaData(m_renderer.get(), m_resBinder.get(), listAreaPtr, 1);
	}
}