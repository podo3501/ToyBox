#include "pch.h"
#include "UIComponentFactory.h"
#include "Component/Panel.h"
#include "Component/ImageGrid1.h"
#include "Component/ImageGrid3.h"
#include "Component/ImageGrid9.h"
#include "Component/Button.h"
#include "Component/TextArea.h"
#include "Component/Dialog.h"
#include "Component/ListArea.h"
#include "Component/Container.h"
#include "Component/RenderTexture.h"
#include "Component/ScrollBar.h"
#include "Component/ScrollSlider.h"
#include "Component/ImageSwitcher.h"

using FactoryFunc = unique_ptr<UIComponent>(*)();   //constexpr�� ���� ������ function(�����Ҵ�)�� �� ������.
constexpr FactoryFunc ComponentFactory[] = //enum�� ���� ��ġ�� �Ǿ�� �Ѵ�. �ƴϸ� if�� �ؾ� �Ѵ�.
{
    []() -> unique_ptr<UIComponent> { return make_unique<Panel>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ImageGrid1>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ImageGrid3>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ImageGrid9>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<Button>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<TextArea>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<Dialog>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ListArea>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<Container>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<RenderTexture>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ScrollBar>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ScrollSlider>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ImageSwitcher>(); },
};

unique_ptr<UIComponent> CreateComponent(const string& typeName)
{
    int componentID = EtoV(StringToEnum<ComponentID>(typeName));
    return ComponentFactory[componentID]();
}