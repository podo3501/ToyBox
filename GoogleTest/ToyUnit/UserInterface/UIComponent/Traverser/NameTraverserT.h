#pragma once
#include "UserInterface/UIModul2T.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

class NameTraverserT : public UIModul2T
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
};

void NameTraverserT::SetUp()
{
	UIModul2T::SetUp();
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
