#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/UserInterface/Button.h"
#include "../Toy/UserInterface/TextArea.h"

namespace ReadJson
{
	TEST_F(ToyTestFixture, ReadDialog)
	{
		unique_ptr<Dialog> dialog = make_unique<Dialog>();
		EXPECT_TRUE(dialog->SetResources(L"UI/Data/Dialog.json"));
		
		m_testScene->AddComponent({ 0.f, 0.f }, move(dialog));

		EXPECT_TRUE(m_renderer->LoadScenes());
	}

	TEST_F(ToyTestFixture, ReadButton)
	{
		unique_ptr<Button> button = make_unique<Button>();
		EXPECT_TRUE(button->SetResources(L"UI/Data/Button.json"));

		m_testScene->AddComponent({ 0.f, 0.f }, move(button));

		EXPECT_TRUE(m_renderer->LoadScenes());
	}

	TEST_F(ToyTestFixture, ReadTextArea)
	{
		//unique_ptr<TextArea> button = make_unique<TextArea>();
		//EXPECT_TRUE(button->SetResources(L"UI/Data/TextArea.json"));
	}
}
