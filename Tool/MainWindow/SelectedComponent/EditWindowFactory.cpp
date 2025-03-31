#include "pch.h"
#include "../Toy/UserInterface/UIType.h"
#include "EditWindowFactory.h"
#include "ComponentEdit/EditImageGrid.h"
#include "ComponentEdit/EditButton.h"
#include "ComponentEdit/EditTextArea.h"
#include "ComponentEdit/EditContainer.h"
#include "ComponentEdit/EditRenderTexture.h"
#include "ComponentEdit/EditListArea.h"
#include "ComponentEdit/EditScrollSlider.h"
#include "ComponentEdit/EditScrollBar.h"
#include "ComponentEdit/EditImageSwitcher.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/Container.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Component/ScrollSlider.h"
#include "../Toy/UserInterface/Component/ScrollBar.h"
#include "../Toy/UserInterface/Component/ImageSwitcher.h"

using FactoryFunction = function<unique_ptr<EditWindow>(UIComponent*, IRenderer*, TextureSourceBinder*, CommandList*)>;
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
    //UIComponent*, CommandList*가 인자로 들어가는 새로운 EditWindow는 여기서 추가

    RegisterEditWindowWithRenderer<EditImageGrid1, ImageGrid1*>(ComponentID::ImageGrid1);
    RegisterEditWindowWithRenderer<EditImageGrid3, ImageGrid3*>(ComponentID::ImageGrid3);
    RegisterEditWindowWithRenderer<EditImageGrid9, ImageGrid9*>(ComponentID::ImageGrid9);
    RegisterEditWindowWithRenderer<EditRenderTexture, RenderTexture*>(ComponentID::RenderTexture);
    RegisterEditWindowWithRenderer<EditListArea, ListArea*>(ComponentID::ListArea);
    RegisterEditWindowWithRenderer<EditScrollSlider, ScrollSlider*>(ComponentID::ScrollSlider);
    RegisterEditWindowWithRenderer<EditScrollBar, ScrollBar*>(ComponentID::ScrollBar);
    RegisterEditWindowWithRenderer<EditImageSwitcher, ImageSwitcher*>(ComponentID::ImageSwitcher);
    //UIComponent*, IRenderer*, CommandList*가 인자로 들어가는 새로운 EditWindow는 여기서 추가
}

unique_ptr<EditWindow> EditWindowFactory::Create(UIComponent* component, IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList)
{
    if (!component) return nullptr;

    ComponentID id = component->GetTypeID();
    auto it = m_factoryMap->find(id);
    if (it != m_factoryMap->end())
        return it->second(component, renderer, sourceBinder, cmdList);

    return nullptr;
}

unique_ptr<EditWindow> EditWindowFactory::CreateEditWindow(UIComponent* component, 
    IRenderer* renderer, TextureSourceBinder* sourceBinder, CommandList* cmdList)
{
    return Create(component, renderer, sourceBinder, cmdList);
}