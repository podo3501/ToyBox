#include "pch.h"
#include "JsonOperation.h"
#include "Config.h"
#include "Utility.h"
#include "../UIComponent/UIComponent.h"
#include "../UINameGenerator.h"

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

template<typename T> //?!? SerializeIO가 있는지확인하는 컨셉으로 바꿔야 함.
static void UpdateJson(T* data, nlohmann::ordered_json& writeJ) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    const auto& curJson = jsOp.GetWrite();

    writeJ.update(curJson);
}