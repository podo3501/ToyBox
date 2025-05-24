#include "pch.h"
#include "JsonOperation.h"
#include "../UIComponent/UIComponent.h"
#include "../UIComponent/UIComponentFactory.h"
#include "../UINameGenerator.h"
#include "Utility.h"

template<typename T> //?!? SerializeIO가 있는지확인하는 컨셉으로 바꿔야 함.
static void UpdateJson(T* data, nlohmann::ordered_json& writeJ) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    const auto& curJson = jsOp.GetWrite();

    writeJ.update(curJson);
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
    if (data == nullptr) return;

    ProcessWriteKey(key, [&data](nlohmann::ordered_json& writeJ) {
        writeJ["Type"] = EnumToString<ComponentID>(data->GetTypeID());
        UpdateJson(data, writeJ); });
}

void JsonOperation::Write(const string& key, UINameGenerator* data)
{
    if (data == nullptr) return;

    ProcessWriteKey(key, [&data](nlohmann::ordered_json& writeJ) {
        UpdateJson(data, writeJ); });
}

void JsonOperation::Process(const string& key, unique_ptr<UIComponent>& data)
{
    if (IsWrite())
        Write(key, data.get());
    else
    {
        //Read(key, data.get()); //?!? 이런 함수가 만들어져야 한다.
        ProcessReadKey(key, [this, &data](auto& currentJson) {
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