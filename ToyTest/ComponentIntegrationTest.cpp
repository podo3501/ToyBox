#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIComponentHelper.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Command/CommandList.h"

namespace ComponentTest
{
	static bool AttachComponentHelper(UIComponent* panel, const string& componentName) noexcept
	{
		auto imgGrid1 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
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
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		m_panel->AttachComponent(move(img9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid9_0"), false);	//9방향 이미지에는 attach 불가
		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid1_0"), true);

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_0"), false);
		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_9"), true); //위에서 ImgGrid1를 attach 했다.
	}

	template <typename T>
	bool VerifyClone(unique_ptr<T> original) 
	{
		if (!original) return false;
		auto clone = original->Clone();

		return CompareUniquePtr(original, clone);
	}

	TEST_F(IntegrationTest, Clone)
	{
		EXPECT_TRUE(VerifyClone(CreateSampleImageGrid1({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleImageGrid3({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleTextArea({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleButton1({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleButton3({ { 220, 190 }, Origin::LeftTop })));
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		std::vector<UIComponent*> components;
		panel->GetComponents(position, components);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponents)
	{
		unique_ptr<UIComponent> img9_0 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		m_panel->AttachComponent(move(img9_0), { 80, 60 });

		EXPECT_TRUE(CheckComponentCount(m_panel.get(), {0, 0}) == 1);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 100, 100 }) == 4);

		unique_ptr<UIComponent> img9_1 = CreateSampleImageGrid9({ { 221, 191 }, Origin::LeftTop });
		m_panel->AttachComponent(move(img9_1), { 88, 66 });

		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
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
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		m_panel->AttachComponent(move(img9), { 80, 60 });

		UIComponent* component = m_panel->GetComponent("ImageGrid1_0");
		EXPECT_FALSE(component->Rename("ImageGrid9_0")); //같은 이름이 있으면 rename이 되지 않는다.

		unique_ptr<UIComponent> newImg9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		auto failed = m_panel->AttachComponent(move(newImg9), { 80, 60 }); //같은 컴포넌트를 attach하면 내부적으로 이름을 생성해 준다.
		EXPECT_TRUE(failed == nullptr);
	}

	TEST_F(IntegrationTest, Undo)
	{
		auto component = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		auto img1 = component.get();
		m_panel->AttachComponent(move(component), { 80, 60 });

		CommandList cmdList;
		cmdList.RelativePosition(img1, { 90, 70 });
		cmdList.RelativePosition(img1, { 100, 80 }); //RelativePosition은 명령이 하나로 합쳐짐

		XMINT2 relativePos{};
		img1->GetRelativePosition(relativePos);
		EXPECT_EQ(relativePos, XMINT2(100, 80));

		cmdList.Undo();
		img1->GetRelativePosition(relativePos);
		EXPECT_EQ(relativePos, XMINT2(80, 60));
		cmdList.Undo();	//Undo 할게 없을때 아무 반응 안함

		cmdList.Redo();
		img1->GetRelativePosition(relativePos);
		EXPECT_EQ(relativePos, XMINT2(100, 80));
		cmdList.Redo(); //Redo 할게 없을때 아무 반응 안함
	}
}