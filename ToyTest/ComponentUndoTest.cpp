#include "pch.h"
#include "ToyTestFixture.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"

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

	TEST_F(UndoRedoTest, Component)
	{
		auto component = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		auto img1 = component.get();
		m_panel->AttachComponent(move(component), { 80, 60 });

		CommandList cmdList;
		cmdList.RelativePosition(img1, { 90, 70 });
		cmdList.RelativePosition(img1, { 100, 80 }); //RelativePosition은 명령이 하나로 합쳐짐

		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetRelativePosition, true), XMINT2(80, 60));
		EXPECT_EQ(UndoRedo(cmdList, img1, &UIComponent::GetRelativePosition, false), XMINT2(100, 80));

		cmdList.Size(img1, { 54, 54 });
		cmdList.Size(img1, { 74, 74 }); //Size은 명령이 하나로 합쳐짐

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
		cmdList.Source(img1, { 20, 20, 74, 74 });
		cmdList.Source(img1, { 30, 30, 84, 84 }); //Source은 명령이 하나로 합쳐짐

		EXPECT_EQ(UndoRedo(cmdList, img1, &ImageGrid1::GetSource, true), Rectangle(10, 10, 64, 64));
		EXPECT_EQ(UndoRedo(cmdList, img1, &ImageGrid1::GetSource, false), Rectangle(30, 30, 84, 84));
	}
}