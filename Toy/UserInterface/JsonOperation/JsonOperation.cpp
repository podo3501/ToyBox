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
JsonOperation::JsonOperation() :
m_wCurrent{ &m_write },
m_rCurrent{ &m_read }
{}

JsonOperation::JsonOperation(const nlohmann::json& read)
{
    m_read = read;
    m_rCurrent = &m_read;
}

bool JsonOperation::IsWrite()
{
    //읽는게 아니면 쓰는 것이다.
    if (m_read.empty())
        return true;

    return false;
}

inline static bool IsJsonFile(const wstring& filename) { return filesystem::path(filename).extension() == ".json"; }
bool JsonOperation::Write(const wstring& filename)
{
    ReturnIfFalse(IsJsonFile(filename));

    ofstream file(GetResourceFullFilename(filename));
    if (!file.is_open())
        return false;
    
    auto& json = GetWriteRoot();
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

    m_read = json::parse(file);

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
    auto writeFunc = [&data](auto& j) { j = JsonTraits<Origin>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<Origin>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string & key, Vector2& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = JsonTraits<Vector2>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<Vector2>::DeserializeFromJson(j); };

    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, wstring& data) noexcept
{
    auto writeFunc = [&data](auto& j) { j = JsonTraits<wstring>::SerializeToJson(data); };
    auto readFunc = [&data](const auto& j) { data = JsonTraits<wstring>::DeserializeFromJson(j); };

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