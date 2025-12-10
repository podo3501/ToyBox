#pragma once
#include "UserInterface/UIModuleT.h"
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Toy/UserInterface/CommandHistory/UserInterface/UICommandHistory.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class UICommandHistoryT_PatchTextureStd1 : public UIModuleT
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	template<typename Action>
	void VerifyUndoRedo(Action&& action, PatchTextureStd1* component = nullptr);
	TextureResourceBinder* GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

	unique_ptr<UICommandHistory> m_command;
	PatchTextureStd1* m_component{ nullptr };
};

void UICommandHistoryT_PatchTextureStd1::SetUp()
{
	UIModuleT::SetUp();

	m_command = make_unique<UICommandHistory>();
	unique_ptr<PatchTextureStd1> owner;
	tie(owner, m_component) = GetPtrs(CreateComponent<PatchTextureStd1>("One32"));
	
	AttachComponent(m_main, move(owner));
	m_uiModule->BindTextureResources();
}

void UICommandHistoryT_PatchTextureStd1::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	AddMockTextureKeys(resBinder, {
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } },
		{ "One48", { L"Texture512.png", TextureSlice::One, { { 0, 0, 48, 48 } } } }
		});
}
