#include "pch.h"
#include "UINameGenerator.h"
#include "AutoNamer.h"
#include "ComponentNameGenerator.h"
#include "../UIComponent/UIType.h"
#include "../JsonOperation/JsonOperation.h"
#include "Utils/StringUtil.h"
#include "Utils/StlUtil.h"

UINameGenerator::~UINameGenerator() = default;
UINameGenerator::UINameGenerator() = default;

UINameGenerator::UINameGenerator(const UINameGenerator& other)
{
    m_regionGens = CopyAssoc(other.m_regionGens);
    m_componentNameGens = CopyAssoc(other.m_componentNameGens);
}

bool UINameGenerator::operator==(const UINameGenerator& other) const noexcept
{
    ReturnIfFalse(CompareUnorderedAssoc(m_regionGens, other.m_regionGens));
    ReturnIfFalse(CompareUnorderedAssoc(m_componentNameGens, other.m_componentNameGens));

    return true;
}

unique_ptr<UINameGenerator> UINameGenerator::Clone() const
{
    return unique_ptr<UINameGenerator>(new UINameGenerator(*this));
}

string UINameGenerator::MakeRegionOf(const string& region) noexcept
{
    auto [name, id] = ExtractNameAndId(region);
    if (name.empty()) {
        TryEmplaceAssoc(m_componentNameGens, name); //���� ���� region�� Ư�� region�̹Ƿ�, nameGenerator���� �������� �ʰ� �׳� ��������.
        return {};
    }

    auto& nameGenerator = TryEmplaceAssoc(m_regionGens, name);
    string newRegion = AppendIfPresent(name, nameGenerator->Generate());
    if (!IsUnusedRegion(newRegion)) return "";

    m_componentNameGens.emplace(newRegion, make_unique<ComponentNameGenerator>());
    return newRegion;
}

bool UINameGenerator::RemoveRegion(const string& region) noexcept
{
    if (IsUnusedRegion(region)) return true;

    auto [name, id] = ExtractNameAndId(region);
    auto find = m_regionGens.find(name);
    if (find == m_regionGens.end()) return false; //Ű���� �ִµ� Generator�� ������ �̻��� ���̴�.

    auto [result, deletable] = find->second->Recycle(id);
    ReturnIfFalse(result);

    m_componentNameGens.erase(region);
    if (deletable) m_regionGens.erase(name);
    return true;
}

bool UINameGenerator::IsUnusedRegion(string_view region) noexcept
{
    return m_componentNameGens.find(region) == m_componentNameGens.end();
}

static bool IsGeneratedComponentName(const string& name, const string& prefix)
{
    if (name.empty()) return true;

    auto [curName, id] = ExtractNameAndId(name);
    return curName == prefix;
}

pair<string, string> UINameGenerator::MakeNameOf(const string& name, const string& region, ComponentID componentID, bool forceUniqueRegion) noexcept
{
    string newRegion{ region }, newName{ name };
    if (IsUnusedRegion(region) || forceUniqueRegion) //���� region�� �������� �ʰų�, �����ϴ��� ����ũ�̾�� �Ѵٸ�
        newRegion = MakeRegionOf(region);

    auto nameGen = GetComponentNameGen(newRegion);
    if (!nameGen) return { "", "" };

    const string& strComponent = EnumToString<ComponentID>(componentID);
    if (IsGeneratedComponentName(name, strComponent))
        newName = nameGen->MakeNameFromComponent(strComponent);
    else
        newName = nameGen->MakeNameFromBase(name);

    return { newRegion, newName };
}

bool UINameGenerator::RemoveName(const string& region, const string& name) noexcept
{
    auto nameGen = GetComponentNameGen(region);
    return nameGen ? nameGen->Remove(name) : true;
}

bool UINameGenerator::IsUnusedName(string_view region, string_view name) noexcept
{
    auto nameGen = GetComponentNameGen(region);
    return nameGen ? nameGen->IsUnusedName(name) : true;
}

ComponentNameGenerator* UINameGenerator::GetComponentNameGen(string_view region) const noexcept
{
    auto find = m_componentNameGens.find(region);
    if (find == m_componentNameGens.end()) return nullptr;

    return find->second.get();
}

void UINameGenerator::SerializeIO(JsonOperation& operation)
{
    operation.Process("RegionGens", m_regionGens);
    operation.Process("RegionNames", m_componentNameGens);
}