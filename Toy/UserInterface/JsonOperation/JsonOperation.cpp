#include "pch.h"
#include "JsonOperation.h"
#include "Config.h"
#include "Utils/CommonUtil.h"
#include "../UIComponent/UIComponent.h"

JsonOperation::~JsonOperation() = default;
JsonOperation::JsonOperation() noexcept :
m_wCurrent{ &m_write },
m_rCurrent{ &m_read }
{}

JsonOperation::JsonOperation(nlohmann::ordered_json& write) noexcept { m_wCurrent = &write; }
JsonOperation::JsonOperation(const nlohmann::json& read) noexcept { m_rCurrent = const_cast<nlohmann::json*>(&read); }

bool JsonOperation::IsWrite()
{
    if (!m_rCurrent || m_rCurrent->empty()) return true; //읽는게 아니면 쓰는 것이다.
    return false;
}

inline static bool IsJsonFile(const wstring& filename) { return filesystem::path(filename).extension() == ".json"; }
bool JsonOperation::Write(const wstring& filename)
{
    ReturnIfFalse(IsJsonFile(filename));

    ofstream file(GetResourceFullFilename(filename));
    if (!file.is_open())
        return false;
    
    auto& json = GetWrite();
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

    (*m_rCurrent) = nlohmann::json::parse(file);

    return true;
}