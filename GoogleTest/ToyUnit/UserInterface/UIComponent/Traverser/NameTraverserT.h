#pragma once
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

class NameTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<UIComponentManager> m_componentManager;
	unique_ptr<UIComponent> m_owner;
	UIComponent* m_main{ nullptr };
};

void NameTraverserT::SetUp()
{
	m_componentManager = make_unique<UIComponentManager>();
	Locator<UIComponentManager>::Provide(m_componentManager.get());

	tie(m_owner, m_main) = CreateMockComponent<MockComponent>();
	Rename(m_main, "Unknown"); //Attach 할때만 이름이 자동 생성된다.
}

//함수 이름이 같을때 처리.
//inline bool RenameRegion() noexcept {
//	return BaseCall(static_cast<bool(BaseTraverser::*)() noexcept>(&BaseTraverser::RenameRegion));
//}
//inline bool RenameRegion(int a) noexcept {
//	return BaseCall(static_cast<bool(BaseTraverser::*)(int) noexcept>(&BaseTraverser::RenameRegion), a);
//}
//inline bool RenameRegion(int a, int b) noexcept {
//	return BaseCall(static_cast<bool(BaseTraverser::*)(int, int) noexcept>(& BaseTraverser::RenameRegion), a, b);
//}
