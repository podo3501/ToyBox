#include "pch.h"
#include "SerializerIO.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include <fstream>

SerializerIO::~SerializerIO() = default;
SerializerIO::SerializerIO() noexcept :
m_wCurrent{ &m_write },
m_rCurrent{ &m_read }
{
    int a = 1;
}

SerializerIO::SerializerIO(nlohmann::ordered_json& write) noexcept { m_wCurrent = &write; }
SerializerIO::SerializerIO(const nlohmann::json& read) noexcept { m_rCurrent = const_cast<nlohmann::json*>(&read); }

bool SerializerIO::IsWrite()
{
    if (!m_rCurrent || m_rCurrent->empty()) return true; //읽는게 아니면 쓰는 것이다.
    return false;
}

inline static bool IsJsonFile(const wstring& filename) { return filesystem::path(filename).extension() == ".json"; }
bool SerializerIO::Write(const wstring& filename)
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

bool SerializerIO::Read(const wstring& filename)
{
    ReturnIfFalse(IsJsonFile(filename));

    ifstream file(GetResourceFullFilename(filename));
    if (!file.is_open())
        return false;

    (*m_rCurrent) = nlohmann::json::parse(file);

    return true;
}