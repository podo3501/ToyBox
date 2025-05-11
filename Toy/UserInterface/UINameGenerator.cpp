#include "pch.h"
#include "UINameGenerator.h"
#include "UIComponent/UIType.h"
#include "Utility.h"
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

////////////////////////////////////////////////////////////////

string ComponentNameGenerator::Create(ComponentID componentID) noexcept
{
    auto componentName = EnumToString<ComponentID>(componentID);
    return componentName + "_" + m_namers[componentID].Generate();
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

////////////////////////////////////////////////////////////////

string UINameGenerator::MakeNameOf(const string& region, ComponentID componentID) noexcept
{
    return m_regionNames[region].Create(componentID);
}

bool UINameGenerator::RemoveNameOf(const string& region, const string& name) noexcept
{
    auto find = m_regionNames.find(region);
    if (find == m_regionNames.end()) return false;

    return find->second.Remove(name);
}