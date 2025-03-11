#include "pch.h"
#include "JsonOperation.h"
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

void JsonOperation::UpdateJson(const unique_ptr<UIComponent>& data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    //static_cast를 해서 json으로 할당하면 json내의 타입값이 바뀌면서 새로운 타입이 되어 이전 정보 (여기서는 position)값이 사라진다. 그래서 update를 해서 병합하는 것.
    if (jsOp.GetWrite().empty())
        return;
    m_write->GetCurrent().update(jsOp.GetWrite());
}

void JsonOperation::UpdateJson(UIComponent* data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    //static_cast를 해서 json으로 할당하면 json내의 타입값이 바뀌면서 새로운 타입이 되어 이전 정보 (여기서는 position)값이 사라진다. 그래서 update를 해서 병합하는 것.
    if (jsOp.GetWrite().empty())
        return;
    m_write->GetCurrent().update(jsOp.GetWrite());
}

using FactoryFunc = unique_ptr<UIComponent>(*)();   //constexpr을 쓰기 때문에 function(동적할당)을 쓸 수없다.
constexpr FactoryFunc ComponentFactory[] = //enum의 값과 일치가 되어야 한다. 아니면 if로 해야 한다.
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

unique_ptr<UIComponent> JsonOperation::CreateComponentFromType(const string& typeName)
{
    int componentID = EtoV(StringToEnum<ComponentID>(typeName));
    unique_ptr<UIComponent> comp = ComponentFactory[componentID]();

    comp->SerializeIO(*this);
    return comp;
}

//?!? Read를 만들고 void JsonOperation::Process(const string& key, unique_ptr<UIComponent>& data) 얘를 지우자
void JsonOperation::Write(const string& key, UIComponent* data)
{
    if (data == nullptr)
        return;

    m_write->GotoKey(key);
    const string& type = EnumToString<ComponentID>(data->GetTypeID());
    Process("Type", const_cast<string&>(type));
    UpdateJson(data);
    m_write->GoBack();
}

void JsonOperation::Process(const string& key, unique_ptr<UIComponent>& data)
{
    if (IsWrite())
    {
        if (data == nullptr)
            return;

        m_write->GotoKey(key);
        const string& type = EnumToString<ComponentID>(data->GetTypeID());
        Process("Type", const_cast<string&>(type));
        UpdateJson(data);
        m_write->GoBack();
    }
    else
    {
        ProcessReadKey(key, [&data, this](auto& currentJson) {
            string curType{ currentJson["Type"] };
            data = move(CreateComponentFromType(curType));
            });
    }
}

void JsonOperation::Process(const string& key, vector<unique_ptr<UIComponent>>& datas)
{
    if (IsWrite())
    {
        ProcessWriteKey(key, [&datas](auto& currentJson) {
            for (auto& comp : datas)
            {
                JsonOperation jsOp{};
                jsOp.Process("Child", comp);
                currentJson.push_back(jsOp.GetWrite());
            }
            });
    }
    else
    {
        datas.clear();
        ProcessReadKey(key, [&datas, this](const auto& currentJson) {
            for (const auto& compJson : currentJson)
            {
                unique_ptr<UIComponent> component = nullptr;
                JsonOperation jsOp{ compJson };
                jsOp.Process("Child", component);
                datas.emplace_back(move(component));
            }
            });
    }
}