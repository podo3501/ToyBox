#include "pch.h"
#include "JsonOperation.h"
#include "../UIComponent/UIComponent.h"
#include "../UIComponent/UIComponentFactory.h"
#include "../UINameGenerator.h"
#include "Utility.h"

//string KeyConverter<wstring>::ToKey(const wstring& key) { return WStringToString(key); }
//wstring KeyConverter<wstring>::FromKey(const string& key) { return StringToWString(key); }

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

void JsonOperation::UpdateJson(UINameGenerator* data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    //static_cast를 해서 json으로 할당하면 json내의 타입값이 바뀌면서 새로운 타입이 되어 이전 정보 (여기서는 position)값이 사라진다. 그래서 update를 해서 병합하는 것.
    if (jsOp.GetWrite().empty())
        return;
    m_write->GetCurrent().update(jsOp.GetWrite());
}

unique_ptr<UIComponent> JsonOperation::CreateComponentFromType(const string& typeName, const nlohmann::json& readJ)
{
    unique_ptr<UIComponent> comp = CreateComponent(typeName);

    JsonOperation jsOp{ readJ };
    comp->SerializeIO(jsOp);
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

void JsonOperation::Write(const string& key, UINameGenerator* data)
{
    if (data == nullptr)
        return;

    m_write->GotoKey(key);
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
            data = move(CreateComponentFromType(curType, currentJson));
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