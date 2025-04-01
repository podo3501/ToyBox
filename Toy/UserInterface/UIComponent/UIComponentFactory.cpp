#include "pch.h"
#include "UIComponentFactory.h"
#include "Components/Panel.h"
#include "Components/ImageGrid1.h"
#include "Components/ImageGrid3.h"
#include "Components/ImageGrid9.h"
#include "Components/Button.h"
#include "Components/TextArea.h"
#include "Components/Dialog.h"
#include "Components/ListArea.h"
#include "Components/Container.h"
#include "Components/RenderTexture.h"
#include "Components/ScrollBar.h"
#include "Components/ScrollSlider.h"
#include "Components/ImageSwitcher.h"

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