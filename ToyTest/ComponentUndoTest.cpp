#include "pch.h"
#include "ToyTestFixture.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"

namespace ComponentTest
{
	//값이 optional로 리턴되는 것도 optional<T> == T가 된다면 EXPECT_EQ(x, y)에서 작동한다.
	template <typename ReturnType, typename ComponentType>
	static ReturnType UndoRedo(CommandList& cmdList, ComponentType* component,
		ReturnType(ComponentType::* getter)() const noexcept, bool undo) noexcept
	{
		undo ? cmdList.Undo() : cmdList.Redo();
		return (component->*getter)();
	}

	static bool UndoRedoAttach(CommandList& cmdList, bool undo) noexcept
	{
		return undo ? cmdList.Undo() : cmdList.Redo();
	}

	TEST_F(UndoRedoTest, Component)
	{
		auto component = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		auto img1 = component.get();

		CommandList cmdList;
		cmdList.AttachComponent(m_panel.get(), move(component), { 80, 60 });
		EXPECT_EQ(UndoRedoAttach(cmdList, true), true);
		EXPECT_EQ(UndoRedoAttach(cmdList, false), true);
		
		cmdList.SetRelativePosition(img1, { 90, 70 });
		cmdList.SetRelativePosition(img1, { 100, 80 }); //RelativePosition은 명령이 하나로 합쳐짐

		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetRelativePosition, true), XMINT2(80, 60));
		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetRelativePosition, false), XMINT2(100, 80));

		cmdList.SetSize(img1, { 54, 54 });
		cmdList.SetSize(img1, { 74, 74 }); //Size은 명령이 하나로 합쳐짐

		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetSize, true), XMUINT2(64, 64));
		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetSize, false), XMUINT2(74, 74));

		cmdList.Rename(img1, "Image1");
		cmdList.Rename(img1, "Rename");

		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetName, true), "Image1");
		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetName, false), "Rename");
	}

	TEST_F(UndoRedoTest, ImageGrid1)
	{
		auto component = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		auto img1 = ComponentCast<ImageGrid1*>(component.get());
		m_panel->AttachComponent(move(component), { 80, 60 });

		CommandList cmdList;
		cmdList.SetSource(img1, { 20, 20, 74, 74 });
		cmdList.SetSource(img1, { 30, 30, 84, 84 }); //Source은 명령이 하나로 합쳐짐

		EXPECT_EQ(UndoRedo(cmdList, img1, &ImageGrid1::GetSource, true), Rectangle(10, 10, 64, 64));
		EXPECT_EQ(UndoRedo(cmdList, img1, &ImageGrid1::GetSource, false), Rectangle(30, 30, 84, 84));

		EXPECT_TRUE(cmdList.SetFilename(img1, m_renderer.get(), L"UI/Texture/option.png"));

		EXPECT_EQ(UndoRedo(cmdList, img1, &ImageGrid1::GetFilename, true), L"UI/SampleTexture/Sample_0.png");
		EXPECT_EQ(UndoRedo(cmdList, img1, &ImageGrid1::GetFilename, false), L"UI/Texture/option.png");
	}

	TEST_F(UndoRedoTest, ImageGrid3)
	{
		auto component = CreateSampleImageGrid3({ { 48, 48 }, Origin::LeftTop });
		auto img3 = ComponentCast<ImageGrid3*>(component.get());
		m_panel->AttachComponent(move(component), { 80, 60 });

		CommandList cmdList;
		EXPECT_TRUE(cmdList.SetFilename(img3, m_renderer.get(), L"UI/Texture/option.png"));

		EXPECT_EQ(UndoRedo(cmdList, img3, &ImageGrid3::GetFilename, true), L"UI/SampleTexture/Sample_0.png");
		EXPECT_EQ(UndoRedo(cmdList, img3, &ImageGrid3::GetFilename, false), L"UI/Texture/option.png");
		
		cmdList.SetSourceAndDivider(img3, { { 20, 92, 58, 58 }, { 25, 31 } }); //원래 위치는 10, 82, 48, 48에 divider 22, 26
		cmdList.SetSourceAndDivider(img3, { { 30, 97, 54, 52 }, { 35, 41 } }); //Merge되어서 첫번째 값은 무시된다.

		EXPECT_EQ(UndoRedo(cmdList, img3, &ImageGrid3::GetSourceAnd2Divider, true), SourceDivider({ 10, 82, 48, 48 }, { 22, 26 }));
		EXPECT_EQ(UndoRedo(cmdList, img3, &ImageGrid3::GetSourceAnd2Divider, false), SourceDivider({ 30, 97, 54, 52 }, { 35, 41 }));
	}

	TEST_F(UndoRedoTest, ImageGrid9)
	{
		auto component = CreateSampleImageGrid9({ { 170, 120 }, Origin::LeftTop });
		auto img9 = ComponentCast<ImageGrid9*>(component.get());
		m_panel->AttachComponent(move(component), { 80, 60 });

		CommandList cmdList;
		EXPECT_TRUE(cmdList.SetFilename(img9, m_renderer.get(), L"UI/Texture/option.png"));

		EXPECT_EQ(UndoRedo(cmdList, img9, &ImageGrid9::GetFilename, true), L"UI/SampleTexture/Sample_0.png");
		EXPECT_EQ(UndoRedo(cmdList, img9, &ImageGrid9::GetFilename, false), L"UI/Texture/option.png");

		cmdList.SetSourceAndDivider(img9, { { 20, 21, 58, 68 }, { 31, 35, 37, 39 } }); //원래 위치는 10, 10, 64, 64에 divider 30, 34, 36, 38
		cmdList.SetSourceAndDivider(img9, { { 30, 25, 64, 66 }, { 33, 38, 38, 40 } });

		EXPECT_EQ(UndoRedo(cmdList, img9, &ImageGrid9::GetSourceAnd4Divider, true), SourceDivider({ 10, 10, 64, 64 }, { 30, 34, 36, 38 }));
		EXPECT_EQ(UndoRedo(cmdList, img9, &ImageGrid9::GetSourceAnd4Divider, false), SourceDivider({ 30, 25, 64, 66 }, { 33, 38, 38, 40 }));
	}
}