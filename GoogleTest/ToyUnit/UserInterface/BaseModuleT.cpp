#include "pch.h"
#include "BaseModuleT.h"
#include "Shared/Utils/GeometryExt.h"
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Toy/UserInterface/UIComponent/UIType.h"

void BaseModuleT::SetUp()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });

	m_renderer = make_unique<MockRenderer>();
	m_renderer->RegisterMockTextureInfos({
		{ L"Texture512.png", 0, {512, 512} }
		});

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	m_uiModule = CreateUIModule(layout, "Main", m_renderer.get());
	auto resBinder = m_uiModule->GetTexResBinder();
	m_main = m_uiModule->GetMainPanel();
}