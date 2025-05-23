#include "pch.h"
#include "JsonOperation.h"
#include "Config.h"
#include "Utility.h"
#include "../UIComponent/UIType.h"
#include "../UIComponent/UITransform.h"
#include "../TextureResourceBinder/TextureResourceBinder.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

JsonOperation::~JsonOperation() = default;
JsonOperation::JsonOperation()
{
    auto writeJson = make_unique<ordered_json>();
    m_write1 = make_unique<JsonNavigator<ordered_json>>(move(writeJson));
}

//JsonOperation::JsonOperation(const nlohmann::ordered_json& write)
//{
//    auto writeJson = make_unique<ordered_json>(write);
//    m_write = make_unique<JsonNavigator<ordered_json>>(move(writeJson));
//}

JsonOperation::JsonOperation(const nlohmann::json& read)
{
    auto readJson = make_unique<json>(read);
    m_read1 = make_unique<JsonNavigator<json>>(move(readJson));
}

bool JsonOperation::IsWrite()
{
    //읽는게 아니면 쓰는 것이다.
    if (m_read1 == nullptr || m_read1->IsEmpty())
        return true;

    return false;
}

//ordered_json& JsonOperation::GetWrite()
//{
//    return m_write->GetCurrent();
//}
//
//json& JsonOperation::GetRead()
//{
//    return m_read->GetCurrent();
//}

bool IsJsonFile(const wstring& filename)
{
    return filesystem::path(filename).extension() == ".json";
}

bool JsonOperation::Write(const wstring& filename)
{
    ReturnIfFalse(IsJsonFile(filename));

    ofstream file(GetResourceFullFilename(filename));
    if (!file.is_open())
        return false;
    
    auto& json = m_write1->GetRoot();
    file << json.dump(4);
    file.close();

    return true;
}

bool JsonOperation::Read(const wstring& filename)
{
    ReturnIfFalse(IsJsonFile(filename));

    ifstream file(GetResourceFullFilename(filename));
    if (!file.is_open())
        return false;

    auto readJson = make_unique<json>(json::parse(file));
    m_read1 = make_unique<JsonNavigator<json>>(move(readJson));

    return true;
}

void JsonOperation::Process(const string& key, XMINT2& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = JsonTraits<XMINT2>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<XMINT2>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, XMUINT2& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = JsonTraits<XMUINT2>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<XMUINT2>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, Rectangle& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = JsonTraits<Rectangle>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<Rectangle>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, Origin& data) noexcept
{
    /*auto writeFunc = [&data](auto& j) { j = EnumToString(data); };
    auto readFunc = [&data](const auto& j) { data = *StringToEnum<Origin>(j); };
    ProcessImpl(key, writeFunc, readFunc);*/

    auto writeFunc = [&data](auto& j) { j = JsonTraits<Origin>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<Origin>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

//static double RoundToSixA(double value) noexcept
//{
//    return round(value * 1e6) / 1e6;
//}

void JsonOperation::Process(const string & key, Vector2& data) noexcept
{
    //auto writeFunc = [&data](auto& j) {
    //    Assert(!isnan(data.x));
    //    Assert(!isnan(data.y));
    //    j["x"] = RoundToSixA(data.x);
    //    j["y"] = RoundToSixA(data.y);
    //    };

    //auto readFunc = [&data](const auto& j) {
    //    json_detail::SafeRead(data.x, j["x"]);
    //    json_detail::SafeRead(data.y, j["y"]);
    //    };

    //ProcessImpl(key, writeFunc, readFunc);

    auto writeFunc = [&data](auto& j) { j = JsonTraits<Vector2>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<Vector2>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, wstring& data) noexcept
{
    //auto writeFunc = [&data](auto& j) { j = WStringToString(data); };
    //auto readFunc = [&data](const auto& j) { data = StringToWString(j); };
    //ProcessImpl(key, writeFunc, readFunc);

    auto writeFunc = [&data](auto& j) { j = JsonTraits<wstring>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<wstring>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, map<InteractState, string>& datas) noexcept
{
    auto writeFunc = [&datas](auto& j) {
    for (const auto& pair : datas)
        j[EnumToString(pair.first)] = pair.second;
    };

    auto readFunc = [&datas](const auto& j) {
    for (const auto& [k, v] : j.items())
        datas.emplace(*StringToEnum<InteractState>(k), v);
    };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, map<int, UITransform>& datas) noexcept 
{
    auto writeFunc = [this, &datas](nlohmann::ordered_json& j) { SerializeMapContainer(datas, j); };
    auto readFunc = [this, &datas](const nlohmann::json& j) { DeserializeMapContainer(j, datas); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, deque<wstring>& data) noexcept
{
    if (IsWrite())
    {
        if (data.empty())
            return;

        ProcessWriteKey(key, [&data](auto& currentJson) {
            for (auto& wstr : data)
                currentJson.push_back(WStringToString(wstr));
            });
    }
    else
    {
        data.clear();
        ProcessReadKey(key, [&data, this](const auto& currentJson) {
            for (const auto& item : currentJson)
                data.emplace_back(StringToWString(item));
            });
    }
}