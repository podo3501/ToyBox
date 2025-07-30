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
    operation.Process("Deletable", m_isDeletable);
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
    if (prefix.empty()) return { "", 0 };

    int id = (idStr.empty()) ? 0 : stoi(string(idStr));
    return { string(prefix), id };
}

ComponentNameGenerator::~ComponentNameGenerator() = default;
ComponentNameGenerator::ComponentNameGenerator() = default;

bool ComponentNameGenerator::operator==(const ComponentNameGenerator& other) const noexcept
{
    ReturnIfFalse(m_namers == other.m_namers);
    return true;
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

bool ComponentNameGenerator::Remove(const string& name) noexcept
{
    auto [baseName, id] = ExtractNameAndId(name);
    if (baseName.empty()) return false;

    auto [result, deletable] = m_namers[baseName].Recycle(id);
    ReturnIfFalse(result);

    if (deletable) m_namers.erase(baseName);

    return true;
}

bool ComponentNameGenerator::IsUniqueName(string_view name) const noexcept
{
    auto [baseName, id] = ExtractNameAndId(name);
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

UINameGenerator::UINameGenerator(const UINameGenerator& other)
{
    m_regionGens = other.m_regionGens;
    m_componentNameGens = other.m_componentNameGens;
}

bool UINameGenerator::operator==(const UINameGenerator& other) const noexcept
{
    ReturnIfFalse(m_regionGens == other.m_regionGens);
    ReturnIfFalse(m_componentNameGens == other.m_componentNameGens);

    return true;
}

unique_ptr<UINameGenerator> UINameGenerator::Clone() const
{
    return unique_ptr<UINameGenerator>(new UINameGenerator(*this));
}

string UINameGenerator::MakeRegionOf(const string& region) noexcept
{
    auto [name, id] = ExtractNameAndId(region);
    auto& nameGenerator = m_regionGens.try_emplace(name).first->second; //first�� iterator, �׵ڿ� ->second�� autonamer

    string newRegion{};
    if (name.empty()) //���� ���� region�� Ư�� region�̹Ƿ�, nameGenerator���� �������� �ʰ� �׳� ��������.
        m_componentNameGens.try_emplace(name);
    else
    {
        newRegion = AppendIfPresent(name, nameGenerator.Generate());
        if (!IsUniqueRegion(newRegion)) return "";

        m_componentNameGens.emplace(newRegion, ComponentNameGenerator{});
    }

    return newRegion;
}

bool UINameGenerator::RemoveRegion(const string& region) noexcept
{
    if (IsUniqueRegion(region)) return true;

    auto [name, id] = ExtractNameAndId(region);
    auto find = m_regionGens.find(name);
    if (find == m_regionGens.end()) return false; //Ű���� �ִµ� Generator�� ������ �̻��� ���̴�.

    auto [result, deletable] = find->second.Recycle(id);
    ReturnIfFalse(result);

    m_componentNameGens.erase(region);
    if (deletable) m_regionGens.erase(name);
    return true;
}

bool UINameGenerator::IsUniqueRegion(string_view region) noexcept
{
    return m_componentNameGens.find(region) == m_componentNameGens.end();
}

static bool ShouldGenerateName(const string& name, const string& prefix)
{
    if (name.empty()) return true;

    auto [curName, id] = ExtractNameAndId(name);
    return curName == prefix;
}

pair<string, string> UINameGenerator::MakeNameOf(const string& name, const string& region, ComponentID componentID, bool forceUniqueRegion) noexcept
{
    string newRegion{ region }, newName{ name };
    if (IsUniqueRegion(region) || forceUniqueRegion) //���� region�� �������� �ʰų�, �����ϴ��� ����ũ�̾�� �Ѵٸ�
        newRegion = MakeRegionOf(region);

    auto find = m_componentNameGens.find(newRegion);
    if (find == m_componentNameGens.end()) return { "", "" };
    
    const string& strComponent = EnumToString<ComponentID>(componentID);
    if (ShouldGenerateName(name, strComponent))
        newName = find->second.MakeNameFromComponent(strComponent);
    else
        newName = find->second.MakeNameFromBase(name);

    return { newRegion, newName };
}

bool UINameGenerator::RemoveName(const string& region, const string& name) noexcept
{
    auto find = m_componentNameGens.find(region);
    if (find == m_componentNameGens.end()) return false;

    return find->second.Remove(name);
}

bool UINameGenerator::IsUniqueName(string_view region, string_view name) noexcept
{
    auto find = m_componentNameGens.find(region);
    if (find == m_componentNameGens.end()) return true;
    return find->second.IsUniqueName(name);
}

void UINameGenerator::SerializeIO(JsonOperation& operation)
{
    operation.Process("RegionGens", m_regionGens);
    operation.Process("RegionNames", m_componentNameGens);
}