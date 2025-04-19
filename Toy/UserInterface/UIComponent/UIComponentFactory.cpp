#include "pch.h"
#include "UIComponentFactory.h"
#include "Components/Panel.h"
#include "Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"
#include "Components/PatchTexture/PatchTextureLite/PatchTextureLite3.h"
#include "Components/PatchTexture/PatchTextureLite/PatchTextureLite9.h"
#include "Components/Button.h"
#include "Components/TextArea.h"
#include "Components/Dialog.h"
#include "Components/ListArea.h"
#include "Components/Container.h"
#include "Components/RenderTexture.h"
#include "Components/ScrollBar.h"
#include "Components/ScrollSlider.h"
#include "Components/TextureSwitcher.h"
#include <iterator>

using FactoryFunc = unique_ptr<UIComponent>(*)();   //constexpr�� ���� ������ function(�����Ҵ�)�� �� ������.
constexpr FactoryFunc ComponentFactory[] = //enum�� ���� ��ġ�� �Ǿ�� �Ѵ�. �ƴϸ� if�� �ؾ� �Ѵ�.
{
    []() -> unique_ptr<UIComponent> { return make_unique<Panel>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<PatchTextureStd1>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<PatchTextureStd3>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<PatchTextureStd9>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<PatchTextureLite1>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<PatchTextureLite3>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<PatchTextureLite9>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<Button>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<TextArea>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<Dialog>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ListArea>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<Container>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<RenderTexture>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ScrollBar>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<ScrollSlider>(); },
    []() -> unique_ptr<UIComponent> { return make_unique<TextureSwitcher>(); },
    //[]() -> unique_ptr<UIComponent> { return make_unique<Unknown>(); } //�̷��� ����.
};

static_assert((size(ComponentFactory) + 1) == EnumSize<ComponentID>(), "ComponentFactory ũ��� ComponentID enum ũ�Ⱑ ��ġ���� �ʽ��ϴ�!");

unique_ptr<UIComponent> CreateComponent(const string& typeName)
{
    int componentID = EtoV(StringToEnum<ComponentID>(typeName));
    return ComponentFactory[componentID]();
}