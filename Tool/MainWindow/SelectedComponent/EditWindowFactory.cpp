#include "pch.h"
#include "EditWindowFactory.h"
#include "ComponentEdit/EditPatchTexture.h"
#include "ComponentEdit/EditButton.h"
#include "ComponentEdit/EditTextArea.h"
#include "ComponentEdit/EditContainer.h"
#include "ComponentEdit/EditRenderTexture.h"
#include "ComponentEdit/EditListArea.h"
#include "ComponentEdit/EditScrollSlider.h"
#include "ComponentEdit/EditScrollBar.h"
#include "ComponentEdit/EditTextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/UIType.h"
#include "../Toy/UserInterface/UIComponent/Components/Panel.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture9.h"
#include "../Toy/UserInterface/UIComponent/Components/Button.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/Container.h"
#include "../Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollSlider.h"
#include "../Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

using FactoryFunction = function<unique_ptr<EditWindow>(UIComponent*, IRenderer*, TextureResourceBinder*, UICommandList*)>;
optional<unordered_map<ComponentID, FactoryFunction>> EditWindowFactory::m_factoryMap{ nullopt };

void EditWindowFactory::RegisterFactories()
{
    if (m_factoryMap) return; //������ ȣ���ص� �ѹ��� ����� �ǰԲ� nullopt(���� ������)�� �ǰԲ� �Ѵ�.
    m_factoryMap.emplace(unordered_map<ComponentID, FactoryFunction>()); //�� ���·� �ʱⰪ�� �����.

    //EditWindow �����Ҷ� ���ڰ� �پ��ϰ� �� �� �־ �̷��� �ߴ�. 
    RegisterEditWindow<EditPanel, Panel*>(ComponentID::Panel);
    RegisterEditWindow<EditButton, Button*>(ComponentID::Button);
    RegisterEditWindow<EditTextArea, TextArea*>(ComponentID::TextArea);
    RegisterEditWindow<EditContainer, Container*>(ComponentID::Container);
    //UIComponent*, UICommandList*�� ���ڷ� ���� ���ο� EditWindow�� ���⼭ �߰�

    RegisterEditWindowWithRenderer<EditPatchTexture1, PatchTexture1*>(ComponentID::PatchTexture1);
    RegisterEditWindowWithRenderer<EditPatchTexture3, PatchTexture3*>(ComponentID::PatchTexture3);
    RegisterEditWindowWithRenderer<EditPatchTexture9, PatchTexture9*>(ComponentID::PatchTexture9);
    RegisterEditWindowWithRenderer<EditRenderTexture, RenderTexture*>(ComponentID::RenderTexture);
    RegisterEditWindowWithRenderer<EditListArea, ListArea*>(ComponentID::ListArea);
    RegisterEditWindowWithRenderer<EditScrollSlider, ScrollSlider*>(ComponentID::ScrollSlider);
    RegisterEditWindowWithRenderer<EditScrollBar, ScrollBar*>(ComponentID::ScrollBar);
    RegisterEditWindowWithRenderer<EditTextureSwitcher, TextureSwitcher*>(ComponentID::TextureSwitcher);
    //UIComponent*, IRenderer*, UICommandList*�� ���ڷ� ���� ���ο� EditWindow�� ���⼭ �߰�
}

unique_ptr<EditWindow> EditWindowFactory::Create(UIComponent* component, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList)
{
    if (!component) return nullptr;

    ComponentID id = component->GetTypeID();
    auto it = m_factoryMap->find(id);
    if (it != m_factoryMap->end())
        return it->second(component, renderer, resBinder, cmdList);

    return nullptr;
}

unique_ptr<EditWindow> EditWindowFactory::CreateEditWindow(UIComponent* component, 
    IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList)
{
    return Create(component, renderer, resBinder, cmdList);
}