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

    return (id == 0) ? "" : to_string(id);
}

pair<bool, bool> AutoNamer::Recycle(int id) noexcept
{
    if (id >= m_nextID) return { false, false };
    if (!InsertIfAbsent(m_recycled, id)) return { false, false };

    return { true, IsDeletable() };
}

bool AutoNamer::IsUsed(int id) const noexcept
{
    bool generated = HasBeenGenerated(id);
    bool notRecycled = (m_recycled.find(id) == m_recycled.end());

    return generated && notRecycled;
}

void AutoNamer::SerializeIO(JsonOperation& operation)
{
    operation.Process("Namers", m_nextID);
    operation.Process("Recycled", m_recycled);
}

////////////////////////////////////////////////////////////////

static inline string AppendIfPresent(const string& base, const string& suffix, const string& delim = "_")
{
    return suffix.empty() ? base : base + delim + suffix;
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

static pair<string, int> ExtractNameAndId(string_view name)
{
    auto [prefix, idStr] = SplitNameAndId(name);
    if (prefix.empty() || idStr.empty()) return { "", -1 };

    if (!IsValidEnumString<ComponentID>(prefix)) return { "", -1 };
    return { string(prefix), stoi(string(idStr)) };
}

static pair<string, int> ExtractNameAndId2(string_view name)
{
    auto [prefix, idStr] = SplitNameAndId(name);
    if (prefix.empty()) return { "", 0 };

    int id = (idStr.empty()) ? 0 : stoi(string(idStr));
    return { string(prefix), id };
}

ComponentNameGenerator::~ComponentNameGenerator() = default;
ComponentNameGenerator::ComponentNameGenerator() = default;

bool ComponentNameGenerator::operator==(const ComponentNameGenerator& other) const noexcept
{
    ReturnIfFalse(ranges::equal(m_namers, other.m_namers));
    return true;
}

string ComponentNameGenerator::MakeRegion() noexcept
{
    return AppendIfPresent("", m_region.Generate());
}

string ComponentNameGenerator::MakeNameFromComponent(const string& name) noexcept
{
    return AppendIfPresent(name, m_namers[name].Generate());
}

string ComponentNameGenerator::MakeNameFromBase(const string& name) noexcept
{
    auto [baseName, idStr] = SplitNameAndId(name);
    string strName = string(baseName);
    auto& autoNamer = m_namers.try_emplace(strName).first->second;

    return AppendIfPresent(strName, autoNamer.Generate());
}

template<typename T>
bool IsVaildEnumType(T type)
{
    constexpr auto size = EnumSize<T>();
    if ( size <= static_cast<size_t>(type))
        return false;

    return true;
}

pair<bool, bool> ComponentNameGenerator::RemoveRegion(int id) noexcept
{
    auto [result, deletable] = m_region.Recycle(id);
    if (result == false) return { false, false };
    
    bool regionDeletable = m_namers.empty() && deletable;
    return { result, regionDeletable };
}

bool ComponentNameGenerator::Remove(const string& name) noexcept
{
    auto [baseName, id] = ExtractNameAndId2(name);
    if (baseName.empty()) return false;

    auto [result, deletable] = m_namers[baseName].Recycle(id);
    ReturnIfFalse(result);

    if (deletable) m_namers.erase(baseName);

    return true;
}

bool ComponentNameGenerator::IsUniqueName(string_view name) const noexcept
{
    auto [baseName, id] = ExtractNameAndId2(name);
    if (baseName.empty()) return true;

    auto find = m_namers.find(baseName);
    if (find == m_namers.end()) return true;

    return !find->second.IsUsed(id);
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

static string GetBaseRegionName(string_view region) 
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
    auto [vName, _] = SplitNameAndId(region);
    string name{ vName };
    auto& nameGenerator = m_componentNameGens.try_emplace(name).first->second;

    return name + nameGenerator.MakeRegion();
}

bool UINameGenerator::TryRemoveRegion(const string& region) noexcept
{
    auto [name, idStr] = SplitNameAndId(region);
    if (name.empty()) return false;

    auto find = m_componentNameGens.find(string(name));
    if (find == m_componentNameGens.end()) return false;

    int id = idStr.empty() ? 0 : stoi(string(idStr));
    auto [result, deletable] = find->second.RemoveRegion(id);
    ReturnIfFalse(result);

    if (deletable) m_componentNameGens.erase(find);
    return true;
}

bool UINameGenerator::IsUniqueRegion(string_view region) noexcept
{
    return m_componentNameGens.find(region) == m_componentNameGens.end();
}

static bool ShouldGenerateName(const string& name, const string& prefix)
{
    if (name.empty()) return true;
    return name.find(prefix) != string::npos;
}

string UINameGenerator::MakeNameOf(const string& name, const string& region, ComponentID componentID) noexcept
{
    const string& strComponent = EnumToString<ComponentID>(componentID);
    if (ShouldGenerateName(name, strComponent))
        return m_componentNameGens[region].MakeNameFromComponent(strComponent);
    else
        return m_componentNameGens[region].MakeNameFromBase(name);
}

bool UINameGenerator::TryRemoveName(const string& region, const string& name) noexcept
{
    auto find = m_componentNameGens.find(region);
    if (find == m_componentNameGens.end()) return false;

    if (find->second.Remove(name) == false) return false;
    if (find->second.Empty()) m_componentNameGens.erase(find);

    return true;
}

bool UINameGenerator::IsUniqueName(string_view region, string_view name) noexcept
{
    auto find = m_componentNameGens.find(region);
    if (find == m_componentNameGens.end()) return true;
    return find->second.IsUniqueName(name);
}

void UINameGenerator::SerializeIO(JsonOperation& operation)
{
    operation.Process("RegionNames", m_componentNameGens);
}