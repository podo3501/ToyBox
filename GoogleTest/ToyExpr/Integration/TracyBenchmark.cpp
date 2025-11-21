#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "IRenderer.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"
#include "Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"

namespace Integration
{
	TEST_F(TracyBenchmark, MakeListAreaData)
	{
		auto [listArea, listAreaPtr] = GetPtrs(CreateSampleListArea({}));
		UITraverser::AttachComponent(m_main, move(listArea), { 400, 300 });
		MakeSampleListAreaData(GetTextureController(), GetResBinder(), listAreaPtr, 20);
	}
}