#include "pch.h"
#include "UINameGenerator.h"
#include "UIComponent/UIType.h"
#include "Utility.h"
#include "StringUtil.h"
#include "JsonOperation/JsonOperation.h"

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
    auto& nameGenerator = m_regionGens.try_emplace(name).first->second; //first는 iterator, 그뒤에 ->second는 autonamer

    string newRegion{};
    if (name.empty()) //값이 없는 region은 특별 region이므로, nameGenerator에서 생성하지 않고 그냥 내보낸다.
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
    if (find == m_regionGens.end()) return false; //키값은 있는데 Generator가 없으면 이상한 일이다.

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
    if (IsUniqueRegion(region) || forceUniqueRegion) //붙일 region이 존재하지 않거나, 존재하더라도 유니크이어야 한다면
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

//?!? util폴더 만들어서 정리하기. UINameGenerator 관련 파일들 폴더에 넣기. 