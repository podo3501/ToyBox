#include "pch.h"
#include "JsonOperation.h"
#include "../Utility.h"
#include "../Config.h"
#include "UIType.h"

using Json = nlohmann::json;
using Ordered_Json = nlohmann::ordered_json;

string WToS(const wstring& data)
{
    return RemoveNullTerminator(WStringToString(data));
}

JsonOperation::JsonOperation()
{
    m_write.push(&m_ordered_json);
}

bool JsonOperation::WriteFile(const wstring& filename) const noexcept
{
    ofstream file(GetResourcePath() + filename);
    if (!file.is_open())
        return false;

    file << m_ordered_json.dump(4);
    file.close();

    return true;
}

bool JsonOperation::ReadFile(const wstring& filename) noexcept
{
    ifstream file(GetResourcePath() + filename);
    if (!file.is_open())
        return false;

    m_json.clear();
    file >> m_json;
    file.close();

    return true;
}

Ordered_Json& JsonOperation::GetWriteJson()
{
    return *m_write.top();
}

void JsonOperation::Push(const string& key) noexcept
{
    if (m_json.empty())
    {
        Ordered_Json& curJson = GetWriteJson();
        curJson[key] = Ordered_Json::array();
        m_write.push(&curJson[key]);
        //curJson[key].push_back({});
    }
}

void JsonOperation::Pop() noexcept
{
    if (m_json.empty())
    {
        //Ordered_Json& curJson = GetWriteJson();
        m_write.pop();
    }
}

void JsonOperation::Process(const string& key, wstring& data) noexcept
{
    if (m_json.empty())
    {
        auto& json = GetWriteJson();
        json[key] = WToS(data);
    }
    else
        data = StringToWString(m_json.at(key).get<std::string>());
}

void JsonOperation::Process(const string& key, map<wstring, wstring>& data) noexcept
{
    if (m_json.empty())
    {
        Ordered_Json j{};
        for (const auto& i : data)
            j[WToS(i.first)] = WToS(i.second);

        auto& json = GetWriteJson();
        json[key] = j;
    }
    else
    {
        const auto& keyJ = m_json[key];
        for (const auto& [k, v] : keyJ.items())
            data.insert(make_pair(StringToWString(k), StringToWString(v)));
    }
}

void JsonOperation::Process(const string& key, Rectangle& data) noexcept
{
    if (m_json.empty())
    {
        Ordered_Json j{};

        j["x"] = data.x;
        j["y"] = data.y;
        j["width"] = data.width;
        j["height"] = data.height;

        auto& json = GetWriteJson();
        if (json.is_array())
            json.push_back(j);
        else
            json[key] = j;
    }
    else
    {
        const auto& keyJ = m_json[key];
        data.x = keyJ["x"];
        data.y = keyJ["y"];
        data.width = keyJ["width"];
        data.height = keyJ["height"];
    }
}

void JsonOperation::Process(const string& key, Origin& data) noexcept
{
    if (m_json.empty())
    {
        auto& json = GetWriteJson();
        json[key] = EnumToString(data);
    }
    else
        data = StringToEnum<Origin>(m_json[key]);
}