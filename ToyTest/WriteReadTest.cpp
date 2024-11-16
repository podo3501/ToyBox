#include "pch.h"
#include "ToyTestFixture.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/UserInterface/Button.h"
#include "../Toy/UserInterface/TextArea.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/JsonHelper.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace WriteReadJson
{
	unique_ptr<UIComponent> CreateTestBGImage(IRenderer* renderer, const string& name, const Rectangle& area)
	{
		UILayout layout(area, Origin::LeftTop);
		ImageSource dialogSource{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
				{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
				{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
			}
		};
		unique_ptr<BGImage> bgImg = make_unique<BGImage>();
		bgImg->SetImage(name, layout, dialogSource);

		return move(bgImg);
	}

	TEST_F(ToyTestFixture, WriteScene)
	{
		unique_ptr<UIComponent> dialog = std::make_unique<Dialog>();
		dialog->SetName("Dialog");
		unique_ptr<UIComponent> bgImg = CreateTestBGImage(m_renderer.get(), "BGImage", { 0, 0, 220, 190 });
		dialog->AddComponent(move(bgImg), { 0.1f, 0.1f });

		m_testScene->AddComponent({ 1.f, 2.f }, move(dialog));
		nlohmann::ordered_json j = *m_testScene;
		WriteJson(j, L"UI/Data/Main.json");

		json readJson = ReadJson(L"UI/Data/Main.json");
		Scene readScene;
		readScene = readJson.get<Scene>();

		EXPECT_TRUE(*m_testScene == readScene);
	}
}
