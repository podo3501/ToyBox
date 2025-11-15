#include "pch.h"
#include "Fixture.h"
#include "IRenderer.h"
#include "Shared/Utils/Profiler.h"
#include "Shared/Window/Window.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/UIComponentManager.h"
#include "Toy/UserInterface/UIModule.h"

Fixture::~Fixture() = default;
Fixture::Fixture() :
	m_main{ nullptr }
{}

void Fixture::SetUp()
{
	m_window = std::make_unique<Window>();

	HWND hwnd{ 0 };
	RECT rc{ 0, 0, 800, 600 };
	EXPECT_TRUE(m_window->Create(GetModuleHandle(nullptr), SW_HIDE, rc, hwnd));
	const auto& outputSize = m_window->GetOutputSize();
	m_environment = InitializeEnvironment(L"../Resources/", outputSize);
	m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y), true);

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";

	m_uiManager = make_unique<UIComponentManager>();
	m_uiModule = m_uiManager->CreateUIModule("Demo", layout, "Main", m_renderer.get(), srcBinderFilename);

	m_uiModule->AddRenderer();
	m_main = m_uiModule->GetMainPanel();

	TracyStartupProfiler();
}

void Fixture::TearDown()
{
	TracyShutdownProfiler();
}

TextureResourceBinder* Fixture::GetResBinder() const noexcept
{ 
	return m_uiModule->GetTexResBinder(); 
}