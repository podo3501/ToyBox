#pragma once
#include "UIComponentManagerT.h"
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/System/StepTimer.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"

class UIModuleT : public UIComponentManagerT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;
	virtual void RegisterBinderTextures(TextureResourceBinder* resBinder) {};
	void SimulateMouse(const XMINT2& pos, InputState state) noexcept;
	void SimulateMouse(int x, int y, InputState state) noexcept;
	void SimulateMouse(const XMINT2& pos, int wheelValue) noexcept;
	void SimulateMouse(int x, int y, int wheelValue) noexcept;
	void SimulateClick(const XMINT2& startPos) noexcept;
	void SimulateDrag(const XMINT2& startPos, const XMINT2& endPos) noexcept;

	UIModule* m_uiModule{ nullptr };
	MockTextureResourceBinder* m_resBindeR{ nullptr };
	Panel* m_main{ nullptr };
};

inline void UIModuleT::SetUp()
{
	UIComponentManagerT::SetUp();

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };

	unique_ptr<MockTextureResourceBinder> resBinder = make_unique<MockTextureResourceBinder>();
	m_resBindeR = resBinder.get();
	m_uiModule = CreateUIModule("Demo", layout, "Main", move(resBinder));

	RegisterBinderTextures(m_uiModule->GetTexResBinder());
	m_main = m_uiModule->GetMainPanel();
}

inline void UIModuleT::TearDown()
{
	m_main = nullptr;
	m_uiModule = nullptr;
	ReleaseUIModule("Demo");

	UIComponentManagerT::TearDown();
}

inline void UIModuleT::SimulateMouse(const XMINT2& pos, InputState state) noexcept
{
	SimulateMouse(pos.x, pos.y, state);
}

inline void UIModuleT::SimulateMouse(int x, int y, InputState state) noexcept
{
	DX::StepTimer timer;
	m_input->SetMouseState(x, y, state);
	m_uiModule->Update(timer);
}

inline void UIModuleT::SimulateMouse(const XMINT2& pos, int wheelValue) noexcept
{
	SimulateMouse(pos.x, pos.y, wheelValue);
}

inline void UIModuleT::SimulateMouse(int x, int y, int wheelValue) noexcept
{
	DX::StepTimer timer;
	m_input->SetMouseState(x, y, InputState::Up);
	m_input->SetMouseWheelValue(wheelValue);
	m_uiModule->Update(timer);
}

inline void UIModuleT::SimulateClick(const XMINT2& startPos) noexcept
{
	SimulateDrag(startPos, startPos);
}

inline void UIModuleT::SimulateDrag(const XMINT2& startPos, const XMINT2& endPos) noexcept
{
	SimulateMouse(startPos, InputState::Pressed);
	SimulateMouse(endPos, InputState::Held);
	SimulateMouse(endPos, InputState::Released);
}