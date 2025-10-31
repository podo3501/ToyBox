#pragma once
#include "UserInterface/BaseModuleT.h"
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/CommandHistory/UserInterface/UICommandHistory.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class UICommandHistoryT_PatchTextureStd1 : public BaseModuleT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(TextureResourceBinder* resBinder) override;

	template<typename Action>
	void VerifyUndoRedo(Action&& action, PatchTextureStd1* component = nullptr);
	TextureResourceBinder* GetResBinder() const noexcept { return m_uiModule->GetTexResBinder(); }

	unique_ptr<UICommandHistory> m_command;
	PatchTextureStd1* m_component{ nullptr };
};

void UICommandHistoryT_PatchTextureStd1::SetUp()
{
	BaseModuleT::SetUp();

	m_command = make_unique<UICommandHistory>();
	unique_ptr<PatchTextureStd1> owner;
	tie(owner, m_component) = GetPtrs(CreateComponent<PatchTextureStd1>("One32"));
	
	UIEx(m_main).AttachComponent(move(owner));
	m_uiModule->BindTextureResources();
}

void UICommandHistoryT_PatchTextureStd1::RegisterBinderTextures(TextureResourceBinder* resBinder)
{
	AddMockTextureKeys(resBinder, {
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } },
		{ "One48", { L"Texture512.png", TextureSlice::One, { { 0, 0, 48, 48 } } } }
		});

	//uiModule������ resBinder�� �����Ǵµ� �׶��� Ű�� ���� �����Ͱ� ��� ���� �ؽ��Ŀ� Binding�� ���� �ʾҴ�.
	m_renderer->LoadTextureBinder(resBinder); 
}
