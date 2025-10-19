#include "pch.h"
#include "FixtureSuite.h"
#include "UserInterface/Helper.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/Locator/EventDispatcherLocator.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/CommandHistory/UserInterface/UICommandHistory.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/CommandHistory/TextureResource/TexResCommandHistory.h"

bool Integration::VerifyClone(unique_ptr<UIComponent> original)
{
	if (!original) return false;
	auto resBinder = m_uiModule->GetTexResBinder();
	EXPECT_TRUE(original->BindTextureSourceInfo(resBinder, m_mockRenderer->GetTextureController()));
	auto clone = original->Clone();

	return Compare(original, clone);
}

//////////////////////////////////////////////////////////////////

void ComponentEvent::SetUp()
{
	UIFixture::SetUp();

	m_eventDispatcherManager = CreateEventDispatcherManager();
	EventDispatcherLocator::Provide(m_eventDispatcherManager.get());
}

void ComponentEvent::TearDown()
{
	UIFixture::TearDown();
}

//////////////////////////////////////////////////////////////////

void TextureSwitcherComponent::FitToTextureSourceTest(const string& bindingKey)
{
	TextureSwitcher* texSwitcher = UIEx(m_main).FindComponent<TextureSwitcher*>("TextureSwitcher");
	EXPECT_TRUE(texSwitcher->FitToTextureSource());
	EXPECT_EQ(texSwitcher->GetSize(), GetSizeOfBindKey(GetResBinder(), bindingKey));
}

void TextureSwitcherComponent::CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey)
{
	unique_ptr<UIComponent> clonePanel = m_main->Clone();
	TestMockRender(2, expectDest, bindKey, clonePanel.get());
	WriteReadTest(GetResBinder(), clonePanel);
	//WriteReadTest(m_resBinder.get(), clonePanel);
}

//////////////////////////////////////////////////////////////////

void UndoRedo::CaptureSnapshot(vector<unique_ptr<UIComponent>>& history)
{
	history.emplace_back(m_main->Clone());
}

void UndoRedo::CaptureSnapshot(vector<unique_ptr<TextureResourceBinder>>& history)
{
	history.emplace_back(make_unique<TextureResourceBinder>(*GetResBinder()));
}

void UndoRedo::VerifyUndoRedo(UICommandHistory& cmdHistory, const vector<unique_ptr<UIComponent>>& history)
{
	for_each(history.rbegin() + 1, history.rend(), [&](const auto& snapshot) {
		cmdHistory.Undo();
		EXPECT_TRUE(*m_main == *snapshot);
		});

	for_each(history.begin() + 1, history.end(), [&](const auto& snapshot) {
		cmdHistory.Redo();
		EXPECT_TRUE(*m_main == *snapshot);
		});
}

void UndoRedo::VerifyUndoRedo(TexResCommandHistory& cmdHistory, const vector<unique_ptr<TextureResourceBinder>>& history)
{
	auto resBinder = GetResBinder();

	for_each(history.rbegin() + 1, history.rend(), [&](const auto& snapshot) {
		cmdHistory.Undo();
		EXPECT_TRUE(*resBinder == *snapshot);
		});

	for_each(history.begin() + 1, history.end(), [&](const auto& snapshot) {
		cmdHistory.Redo();
		EXPECT_TRUE(*resBinder == *snapshot);
		});
}