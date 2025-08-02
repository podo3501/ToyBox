#include "pch.h"
#include "EditWindowFactory.h"
#include "ComponentEdit/EditPatchTexture/EditPatchTextureStd.h"
#include "ComponentEdit/EditPatchTexture/EditPatchTextureLite.h"
#include "ComponentEdit/EditButton.h"
#include "ComponentEdit/EditTextArea.h"
#include "ComponentEdit/EditContainer.h"
#include "ComponentEdit/EditRenderTexture.h"
#include "ComponentEdit/EditListArea.h"
#include "ComponentEdit/EditScrollBar.h"
#include "ComponentEdit/EditTextureSwitcher.h"
#include "UserInterface/UIComponent/Components/Panel.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite3.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite9.h"
#include "UserInterface/UIComponent/Components/Button.h"
#include "UserInterface/UIComponent/Components/TextArea.h"
#include "UserInterface/UIComponent/Components/RenderTexture.h"
#include "UserInterface/UIComponent/Components/Container.h"
#include "UserInterface/UIComponent/Components/ListArea.h"
#include "UserInterface/UIComponent/Components/ScrollBar.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"

using FactoryFunction = function<unique_ptr<EditWindow>(UIComponent*, UICommandList*, TextureResourceBinder*)>;
optional<unordered_map<ComponentID, FactoryFunction>> EditWindowFactory::m_factoryMap{ nullopt };

void EditWindowFactory::RegisterFactories()
{
    if (m_factoryMap) return; //������ ȣ���ص� �ѹ��� ����� �ǰԲ� nullopt(���� ������)�� �ǰԲ� �Ѵ�.
    m_factoryMap.emplace(unordered_map<ComponentID, FactoryFunction>()); //�� ���·� �ʱⰪ�� �����.

    //UIComponent*, UICommandList*�� ���ڷ� ���� ���ο� EditWindow�� ���⼭ �߰�
    RegisterEditWindow<EditPanel, Panel*>(ComponentID::Panel);
    RegisterEditWindow<EditButton, Button*>(ComponentID::Button);
    RegisterEditWindow<EditContainer, Container*>(ComponentID::Container);
    RegisterEditWindow<EditListArea, ListArea*>(ComponentID::ListArea);
    RegisterEditWindow<EditPatchTextureLite1, PatchTextureLite1*>(ComponentID::PatchTextureLite1);
    RegisterEditWindow<EditPatchTextureLite3, PatchTextureLite3*>(ComponentID::PatchTextureLite3);
    RegisterEditWindow<EditPatchTextureLite9, PatchTextureLite9*>(ComponentID::PatchTextureLite9);
    RegisterEditWindow<EditRenderTexture, RenderTexture*>(ComponentID::RenderTexture);
    RegisterEditWindow<EditScrollBar, ScrollBar*>(ComponentID::ScrollBar);
    RegisterEditWindow<EditTextArea, TextArea*>(ComponentID::TextArea);

    //UIComponent*, UICommandList*, TextureResourceBinder*�� ���ڷ� ���� ���ο� EditWindow�� ���⼭ �߰�
    RegisterEditWindowWithBinder<EditPatchTextureStd1, PatchTextureStd1*>(ComponentID::PatchTextureStd1);
    RegisterEditWindowWithBinder<EditPatchTextureStd3, PatchTextureStd3*>(ComponentID::PatchTextureStd3);
    RegisterEditWindowWithBinder<EditPatchTextureStd9, PatchTextureStd9*>(ComponentID::PatchTextureStd9);
    RegisterEditWindowWithBinder<EditTextureSwitcher, TextureSwitcher*>(ComponentID::TextureSwitcher);

    //�ٸ� ���ڸ� �� �ְ� �ȴٸ� RegisterEditWindow�� �������ڸ� �ְ� ���� RegisterEditWindow �Լ��� �ϳ��� ������.
}

unique_ptr<EditWindow> EditWindowFactory::CreateEditWindow(UIComponent* component, 
    UICommandList* cmdList, TextureResourceBinder* resBinder)
{
    if (!component) return nullptr;

    ComponentID id = component->GetTypeID();
    auto it = m_factoryMap->find(id);
    if (it != m_factoryMap->end())
        return it->second(component, cmdList, resBinder);

    return nullptr;
}