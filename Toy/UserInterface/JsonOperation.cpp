#include "pch.h"
#include "JsonOperation.h"
#include "../Config.h"
#include "../Utility.h"
#include "UIType.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

JsonOperation::~JsonOperation() = default;
JsonOperation::JsonOperation()
{
    auto write = std::make_unique<ordered_json>();
    m_write = make_unique<JsonNavigator<ordered_json>>(move(write));
}

bool JsonOperation::IsWrite()
{
    //읽는게 아니면 쓰는 것이다.
    if (m_read == nullptr || m_read->IsEmpty())
        return true;

    return false;
}

ordered_json& JsonOperation::GetWrite()
{
    return m_write->GetCurrent();
}

json& JsonOperation::GetRead()
{
    return m_read->GetCurrent();
}

bool JsonOperation::Write(const wstring& filename)
{
    ofstream file(GetResourcePath() + filename);
    if (!file.is_open())
        return false;

    auto& json = m_write->GetRoot();
    file << json.dump(4);
    file.close();

    return true;
}

bool JsonOperation::Read(const wstring& filename)
{
    ifstream file(GetResourcePath() + filename);
    if (!file.is_open())
        return false;

    auto readJson = make_unique<json>(json::parse(file));
    m_read = make_unique<JsonNavigator<json>>(move(readJson));

    return true;
}

void JsonOperation::ProcessImpl(const string& key, auto writeFunc, auto readFunc)
{
    if (IsWrite())
    {
        m_write->GotoKey(key);
        writeFunc(m_write->GetCurrent());
        m_write->GoBack();
    }
    else
    {
        m_read->GotoKey(key);
        readFunc(m_read->GetCurrent());
        m_read->GoBack();
    }
}

void JsonOperation::Process(const string& key, Rectangle& data) noexcept
{
    auto writeFunc = [&data](auto& j) {
        j["x"] = data.x;
        j["y"] = data.y;
        j["width"] = data.width;
        j["height"] = data.height;
        };

    auto readFunc = [&data](const auto& j) {
        data.x = j["x"];
        data.y = j["y"];
        data.width = j["width"];
        data.height = j["height"];
        };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, Origin& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = EnumToString(data); };
    auto readFunc = [&data](const auto& j) { data = StringToEnum<Origin>(j); };
    ProcessImpl(key, writeFunc, readFunc);
}

double RoundToSixA(double value)
{
    return std::round(value * 1e6) / 1e6;
}

void JsonOperation::Process(const string & key, Vector2& data) noexcept
{
    auto writeFunc = [&data](auto& j) {
        j["x"] = RoundToSixA(data.x);
        j["y"] = RoundToSixA(data.y);
        };

    auto readFunc = [&data](const auto& j) {
        data.x = j["x"];
        data.y = j["y"];
        };

    ProcessImpl(key, writeFunc, readFunc);
}

string RemoveNullWToSA(const wstring& data) noexcept
{
    return RemoveNullTerminator(WStringToString(data));
}

void JsonOperation::Process(const string& key, wstring& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = RemoveNullWToSA(data); };
    auto readFunc = [&data](const auto& j) { data = StringToWString(j); };
    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, map<wstring, wstring>& data) noexcept
{
    auto writeFunc = [&data](auto& j) {
        for (const auto& font : data)
            j[RemoveNullWToSA(font.first)] = RemoveNullWToSA(font.second);
        };

    auto readFunc = [&data](const auto& j) {
        for (const auto& [k, v] : j.items())
            data.insert(make_pair(StringToWString(k), StringToWString(v)));
        };

    ProcessImpl(key, writeFunc, readFunc);
}