#include "pch.h"
#include "JsonOperation.h"
#include "../Config.h"
#include "../Utility.h"
#include "UIType.h"
#include "TransformComponent.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

JsonOperation::~JsonOperation() = default;
JsonOperation::JsonOperation()
{
    auto write = std::make_unique<ordered_json>();
    m_write = make_unique<JsonNavigator<ordered_json>>(move(write));
}

JsonOperation::JsonOperation(const nlohmann::json& read)
{
    auto readJson = make_unique<json>(read);
    m_read = make_unique<JsonNavigator<json>>(move(readJson));
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

    auto& json = m_write->GetRoot();
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

//xy가 들어가는 것은 이걸로 바꾸자.
static void WriteXY(const XMINT2& data, auto& j) { j["x"] = data.x; j["y"] = data.y; }
static void WriteXY(const XMUINT2& data, auto& j) { j["x"] = data.x; j["y"] = data.y; }
static void ReadXY(XMINT2& data, const auto& j) { data.x = j["x"]; data.y = j["y"]; }
static void ReadXY(XMUINT2& data, const auto& j) { data.x = j["x"]; data.y = j["y"]; }

void JsonOperation::Process(const string& key, XMINT2& data) noexcept
{
    auto writeFunc = [&data](auto& j) { WriteXY(data, j); };
    auto readFunc = [&data](const auto& j) { ReadXY(data, j); };
    ProcessImpl(key, writeFunc, readFunc);
}

void JsonOperation::Process(const string& key, XMUINT2& data) noexcept
{
    auto writeFunc = [&data](auto& j) {
        j["x"] = data.x;
        j["y"] = data.y;
    };

    auto readFunc = [&data](const auto& j) {
        data.x = j["x"];
        data.y = j["y"];
    };

    ProcessImpl(key, writeFunc, readFunc);
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

static double RoundToSixA(double value) noexcept
{
    return round(value * 1e6) / 1e6;
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

static string RemoveNullWToSA(const wstring& data) noexcept
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

void JsonOperation::Process(const string& key, map<int, TransformComponent>& datas) noexcept
{
    if (IsWrite())
    {
        if (datas.empty())
            return;

        ProcessWriteKey(key, [&datas](auto& currentJson) {
            for (auto& [k, v] : datas)
            {
                JsonOperation jsOp{};
                v.SerializeIO(jsOp);
                
                JsonOperation map{}; //새로운 operation에 데이터를 넣은뒤에 push_back으로 넣어준다.
                auto& writeJson = map.GetWrite();
                writeJson[to_string(k)] = jsOp.GetWrite();

                currentJson.push_back(map.GetWrite());
            }
            });
    }
    else
    {
        datas.clear();
        ProcessReadKey(key, [&datas, this](const auto& currentJson) {
            for (const auto& j : currentJson) //그냥 디버깅 창을 유심히 보자.
            {
                for (const auto& [k, v] : j.items()) //배열값에서 다시 map 값으로 가져온다.
                {
                    TransformComponent data{};
                    JsonOperation jsOp{ v };
                    data.SerializeIO(jsOp);
                    datas.insert(make_pair(stoi(k), data));
                }
            }
            });
    }
}

void JsonOperation::Process(const string& key, deque<wstring>& data) noexcept
{
    if (IsWrite())
    {
        if (data.empty())
            return;

        ProcessWriteKey(key, [&data](auto& currentJson) {
            for (auto& wstr : data)
                currentJson.push_back(RemoveNullWToSA(wstr));
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