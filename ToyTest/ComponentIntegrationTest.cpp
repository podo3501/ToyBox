#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UIComponentHelper.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"

namespace ComponentTest
{
	static unique_ptr<UIComponent> CreateTestImageGrid9(IRenderer* renderer, const XMUINT2& size)
	{
		UILayout layout(size, Origin::LeftTop);
		ImageSource source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
				{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
				{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
			}
		};
		return CreateImageGrid<ImageGrid9>(layout, source);
	}

	static unique_ptr<UIComponent> CreateTestImageGrid1(IRenderer* renderer, const XMUINT2& size)
	{
		UILayout layout({ 64, 64 }, Origin::LeftTop);
		ImageSource grid1Source{ L"UI/Blue/button_square_header_large_square_screws.png", { { 0, 0, 64, 64 } } };

		return CreateImageGrid<ImageGrid1>(layout, grid1Source);
	}

	static bool AttachComponentHelper(UIComponent* panel, const string& componentName) noexcept
	{
		UILayout layout({ 64, 64 }, Origin::LeftTop);
		ImageSource grid1Source{ L"UI/Blue/button_square_header_large_square_screws.png", { { 0, 0, 64, 64 } } };

		auto imgGrid1 = CreateImageGrid<ImageGrid1>(layout, grid1Source);
		UIComponent* component = panel->GetComponent(componentName);
		return component->AttachComponent(std::move(imgGrid1), { 10, 10 }) ? false : true;
	}

	static bool DetachComponentHelper(UIComponent* panel, const string& componentName) noexcept 
	{
		UIComponent* component = panel->GetComponent(componentName);
		return (component->DetachComponent()).has_value();
	}

	TEST_F(IntegrationTest, AttachDetachTest)
	{
		unique_ptr<UIComponent> img9 = CreateTestImageGrid9(m_renderer.get(), { 220, 190 });
		m_panel->AttachComponent(move(img9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid9_0"), false);	//9방향 이미지에는 attach 불가
		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid1_0"), true);

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_0"), false);
		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_9"), true); //위에서 ImgGrid1를 attach 했다.
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		std::vector<UIComponent*> components;
		panel->GetComponents(position, components);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponents)
	{
		unique_ptr<UIComponent> img9_0 = CreateTestImageGrid9(m_renderer.get(), { 220, 190 });
		m_panel->AttachComponent(move(img9_0), { 80, 60 });

		EXPECT_TRUE(CheckComponentCount(m_panel.get(), {0, 0}) == 1);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 100, 100 }) == 4);

		unique_ptr<UIComponent> img9_1 = CreateTestImageGrid9(m_renderer.get(), { 221, 191 });
		m_panel->AttachComponent(move(img9_1), { 88, 66 });

		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		unique_ptr<UIComponent> img9 = CreateTestImageGrid9(m_renderer.get(), { 220, 190 });
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		panel->AttachComponent(move(img9), { 40, 30 });
		m_panel->AttachComponent(move(panel), { 400, 300 });
		m_panel->RefreshPosition();

		UIComponent* component = m_panel->GetComponent("ImageGrid1_1");
		XMINT2 pos = component->GetPosition();
		EXPECT_EQ(pos, XMINT2(270, 216));
		EXPECT_EQ(GetRectangle(component), Rectangle(270, 216, 160, 128));
	}

	TEST_F(IntegrationTest, Rename)
	{
		unique_ptr<UIComponent> img9 = CreateTestImageGrid9(m_renderer.get(), { 220, 190 });
		m_panel->AttachComponent(move(img9), { 80, 60 });

		UIComponent* component = m_panel->GetComponent("ImageGrid1_0");
		EXPECT_FALSE(component->Rename("ImageGrid9_0"));

		unique_ptr<UIComponent> newImg9 = CreateTestImageGrid9(m_renderer.get(), { 220, 190 });
		auto failed = m_panel->AttachComponent(move(newImg9), { 80, 60 });
		EXPECT_TRUE(failed == nullptr);
	}
}