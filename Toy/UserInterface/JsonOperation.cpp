#include "pch.h"
#include "JsonOperation.h"
#include "../Utility.h"
#include "../Config.h"

using Json = nlohmann::json;

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

void JsonOperation::Process(const std::string& key, std::wstring& data) noexcept
{
    if (m_json.contains(key)) 
        data = StringToWString(m_json.at(key).get<std::string>());
    else 
        m_ordered_json[key] = RemoveNullTerminator(WStringToString(data));
}