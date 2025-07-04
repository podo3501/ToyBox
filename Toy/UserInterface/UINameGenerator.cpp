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

pair<bool, bool> AutoNamer::Recycle(int id) noexcept
{
    if (id >= m_nextID) return { false, false };
    m_recycled.insert(id);
        
    return { true, IsDeletable() };
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

static pair<string_view, string_view> SplitNameAndId(string_view name) 
{
    auto underscore = name.find('_');
    if (underscore == string_view::npos) return { name, {} };

    auto prefix = name.substr(0, underscore);
    auto idStr = name.substr(underscore + 1);
    if (idStr.empty() || !std::ranges::all_of(idStr, ::isdigit)) return {};

    return { prefix, idStr };
}

bool ComponentNameGenerator::Remove(const string& name) noexcept
{
    auto [prefix, idStr] = SplitNameAndId(name);
    if (prefix.empty()) return false;

    auto componentID = StringToEnum<ComponentID>(prefix);
    if (!componentID) return false;

    auto [result, deletable] = m_namers[*componentID].Recycle(stoi(string(idStr)));
    if (deletable) m_namers.erase(*componentID);

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

string UINameGenerator::MakeRegionOf(const string& region) noexcept
{
    string baseRegion = GetBaseRegionName(region);

    auto find = m_regionNameGens.find(baseRegion);
    if (find == m_regionNameGens.end())
    {
        m_regionNameGens.emplace(baseRegion, RegionAutoNamer(baseRegion == region));
        return region;
    }
    
    return baseRegion + "_" + m_regionNameGens[baseRegion].namer.Generate();
}

bool UINameGenerator::TryMarkRegionDeleted(unordered_map<string, RegionAutoNamer>::iterator iter) noexcept
{
    auto& regionAutoNamer = iter->second;
    if (regionAutoNamer.deleted) return false;

    regionAutoNamer.deleted = true;

    if (regionAutoNamer.namer.IsDeletable())
        m_regionNameGens.erase(iter);

    return true;
}

bool UINameGenerator::TryRemoveRegion(const string& region) noexcept
{
    auto [prefix, idStr] = SplitNameAndId(region);
    if (prefix.empty()) return false;

    auto find = m_regionNameGens.find(string(prefix));
    if (find == m_regionNameGens.end()) return false;

    if (idStr.empty())
        return TryMarkRegionDeleted(find);

    auto& regionAutoNamer = find->second;
    auto [result, deletable] = regionAutoNamer.namer.Recycle(stoi(string(idStr)));
    if (deletable && regionAutoNamer.deleted) m_regionNameGens.erase(find);

    return true;
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