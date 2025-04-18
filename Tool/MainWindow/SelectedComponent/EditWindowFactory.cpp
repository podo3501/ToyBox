#include "pch.h"
#include "EditWindowFactory.h"
#include "ComponentEdit/EditPatchTextureStd.h"
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
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
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
    if (m_factoryMap) return; //여러번 호출해도 한번만 등록이 되게끔 nullopt(값이 없을때)만 되게끔 한다.
    m_factoryMap.emplace(unordered_map<ComponentID, FactoryFunction>()); //빈 상태로 초기값을 만든다.

    //EditWindow 생성할때 인자가 다양하게 들어갈 수 있어서 이렇게 했다. 
    RegisterEditWindow<EditPanel, Panel*>(ComponentID::Panel);
    RegisterEditWindow<EditButton, Button*>(ComponentID::Button);
    RegisterEditWindow<EditTextArea, TextArea*>(ComponentID::TextArea);
    RegisterEditWindow<EditContainer, Container*>(ComponentID::Container);
    //UIComponent*, UICommandList*가 인자로 들어가는 새로운 EditWindow는 여기서 추가

    RegisterEditWindowWithRenderer<EditPatchTextureStd1, PatchTextureStd1*>(ComponentID::PatchTextureStd1);
    RegisterEditWindowWithRenderer<EditPatchTextureStd3, PatchTextureStd3*>(ComponentID::PatchTextureStd3);
    RegisterEditWindowWithRenderer<EditPatchTextureStd9, PatchTextureStd9*>(ComponentID::PatchTextureStd9);
    RegisterEditWindowWithRenderer<EditRenderTexture, RenderTexture*>(ComponentID::RenderTexture);
    RegisterEditWindowWithRenderer<EditListArea, ListArea*>(ComponentID::ListArea);
    RegisterEditWindowWithRenderer<EditScrollSlider, ScrollSlider*>(ComponentID::ScrollSlider);
    RegisterEditWindowWithRenderer<EditScrollBar, ScrollBar*>(ComponentID::ScrollBar);
    RegisterEditWindowWithRenderer<EditTextureSwitcher, TextureSwitcher*>(ComponentID::TextureSwitcher);
    //UIComponent*, IRenderer*, UICommandList*가 인자로 들어가는 새로운 EditWindow는 여기서 추가
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