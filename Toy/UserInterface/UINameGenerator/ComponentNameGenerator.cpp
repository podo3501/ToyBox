#include "pch.h"
#include "ComponentNameGenerator.h"
#include "Utility.h"
#include "Utils/StringUtil.h"
#include "../JsonOperation/JsonOperation.h"

ComponentNameGenerator::~ComponentNameGenerator() = default;
ComponentNameGenerator::ComponentNameGenerator() = default;

ComponentNameGenerator::ComponentNameGenerator(const ComponentNameGenerator& other)
{
    for (const auto& [key, value] : other.m_namers)
        m_namers.emplace(key, value ? make_unique<AutoNamer>(*value) : nullptr);
}

bool ComponentNameGenerator::operator==(const ComponentNameGenerator& other) const noexcept
{
    return CompareUnorderedAssoc(m_namers, other.m_namers);
}

ComponentNameGenerator& ComponentNameGenerator::operator=(const ComponentNameGenerator& other)
{
    if (this == &other) return *this;
    
    m_namers.clear();
    for (const auto& [key, value] : other.m_namers)
        m_namers.emplace(key, value ? make_unique<AutoNamer>(*value) : nullptr);

    return *this;
}

//Unique Pointer 일 경우에는 make_unique 해야 하는데 그때 키 값과 인자가 매번 복잡하게 들어가야 하고, 
// try_emplace 구문상 first->second 라는게 헤깔리게 만들 수 있기 때문에 helper 함수 필요
template<typename T, typename Map, typename Key, typename... Args>
    requires is_same_v<typename Map::mapped_type, unique_ptr<T>>
auto& TryEmplaceUPtr(Map& map, const Key& key, Args&&... args)
{
    return map.try_emplace(key, make_unique<T>(forward<Args>(args)...)).first->second;
}

string ComponentNameGenerator::MakeNameFromComponent(const string& name) noexcept
{
    //auto value = TryEmplace(m_namers, name);
    //return AppendIfPresent(name, m_namers[name]->Generate());

    auto& autoNamer = TryEmplaceUPtr<AutoNamer>(m_namers, name);
    return AppendIfPresent(name, autoNamer->Generate());
}

string ComponentNameGenerator::MakeNameFromBase(const string& name) noexcept
{
    auto [baseName, _] = ExtractNameAndId(name);
    auto& autoNamer = TryEmplaceUPtr<AutoNamer>(m_namers, baseName);

    return AppendIfPresent(baseName, autoNamer->Generate());
}

template<typename T>
bool IsVaildEnumType(T type)
{
    constexpr auto size = EnumSize<T>();
    if (size <= static_cast<size_t>(type))
        return false;

    return true;
}

bool ComponentNameGenerator::Remove(const string& name) noexcept
{
    auto [baseName, id] = ExtractNameAndId(name);
    if (baseName.empty()) return false;

    auto [result, deletable] = m_namers[baseName]->Recycle(id);
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

    return !find->second->IsUsed(id);
}

void ComponentNameGenerator::SerializeIO(JsonOperation& operation)
{
    operation.Process("Namers", m_namers);
}