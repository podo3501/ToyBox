#include "pch.h"
#include "UINameGenerator.h"
#include "UIComponent/UIType.h"
#include "Utility.h"
#include "JsonOperation/JsonOperation.h"

AutoNamer::~AutoNamer() = default;
AutoNamer::AutoNamer() = default;

bool AutoNamer::operator==(const AutoNamer& o) const noexcept
{
    ReturnIfFalse(tie(m_nextID, m_recycled) == tie(o.m_nextID, o.m_recycled));
    return true;
}

string AutoNamer::Generate() noexcept
{
    int id;
    if (!m_recycled.empty())
    {
        auto it = m_recycled.begin();
        id = *it;
        m_recycled.erase(it);
    }
    else
        id = m_nextID++;

    return to_string(id);
}

void AutoNamer::Recycle(int id) noexcept
{
    if (id >= m_nextID) return;
    m_recycled.insert(id);
}

void AutoNamer::SerializeIO(JsonOperation& operation)
{
    operation.Process("Namers", m_nextID);
    operation.Process("Recycled", m_recycled);
}

////////////////////////////////////////////////////////////////

ComponentNameGenerator::~ComponentNameGenerator() = default;
ComponentNameGenerator::ComponentNameGenerator() = default;

bool ComponentNameGenerator::operator==(const ComponentNameGenerator& other) const noexcept
{
    ReturnIfFalse(ranges::equal(m_namers, other.m_namers));
    return true;
}

string ComponentNameGenerator::Create(const string& prefix, ComponentID id) noexcept
{
    return prefix + m_namers[id].Generate();
}

template<typename T>
bool IsVaildEnumType(T type)
{
    constexpr auto size = EnumSize<T>();
    if ( size <= static_cast<size_t>(type))
        return false;

    return true;
}

bool ComponentNameGenerator::Remove(const string& name) noexcept
{
    auto underscore = name.find('_');
    if (underscore == string::npos) return false;

    EnumSize<ComponentID>();
    string type = name.substr(0, underscore);
    auto componentID = StringToEnum<ComponentID>(type);
    if (!componentID) return false;

    auto idStr = name.substr(underscore + 1);
    ReturnIfFalse(!idStr.empty() && ranges::all_of(idStr, ::isdigit)); //idStr이 숫자문자 이어야 한다.

    m_namers[*componentID].Recycle(stoi(idStr));
    return true;
}

void ComponentNameGenerator::SerializeIO(JsonOperation& operation)
{
    operation.Process("Namers", m_namers);
}

////////////////////////////////////////////////////////////////

UINameGenerator::~UINameGenerator() = default;
UINameGenerator::UINameGenerator() = default;

bool UINameGenerator::operator==(const UINameGenerator& other) const noexcept
{
    ReturnIfFalse(ranges::equal(m_componentNameGens, other.m_componentNameGens));
    return true;
}

static string GetBaseRegionName(std::string_view region) 
{
    auto pos = region.rfind('_');
    if (pos == string_view::npos || pos + 1 >= region.size()) return string(region);

    auto suffix = region.substr(pos + 1);
    if (ranges::all_of(suffix, [](char c) { return isdigit(c); }))
        return string(region.substr(0, pos));   // 뒤가 전부 숫자라면 _숫자 를 떼고 앞만 리턴
    return string(region);
}

//이거 일단 componentNameGenerator를 흉내내서 만든다음에 두 클래스의 공통 부분을 빼서 만들자.
class NameGenerator
{
public:
private:
};

string UINameGenerator::MakeRegionOf(const string& region) noexcept
{
    string baseRegion = GetBaseRegionName(region);

    auto find = m_regionNameGens.find(baseRegion);
    if (find == m_regionNameGens.end())
    {
        m_regionNameGens.emplace(region, AutoNamer());
        return region;
    }
    
    return region + "_" + m_regionNameGens[region].Generate();
}

static bool ShouldGenerateName(const string& name, const string& prefix)
{
    if (name.empty()) return true;
    return name.find(prefix) != string::npos;
}

string UINameGenerator::MakeNameOf(const string& name, const string& region, ComponentID componentID) noexcept
{
    const string& prefix = EnumToString<ComponentID>(componentID) + "_";
    if (!ShouldGenerateName(name, prefix))
        return name;

    return m_componentNameGens[region].Create(prefix, componentID);
}

bool UINameGenerator::TryRemoveName(const string& region, const string& name) noexcept
{
    auto find = m_componentNameGens.find(region);
    if (find == m_componentNameGens.end()) return false;

    return find->second.Remove(name);
}

void UINameGenerator::SerializeIO(JsonOperation& operation)
{
    operation.Process("RegionNames", m_componentNameGens);
}