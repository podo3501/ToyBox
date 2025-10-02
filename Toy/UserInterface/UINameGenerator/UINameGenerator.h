#pragma once
#include "Shared/Utils/StlTypeExt.hpp"

class JsonOperation;
class AutoNamer;
class ComponentNameGenerator;
enum class ComponentID;
class UINameGenerator
{
protected:
    UINameGenerator(const UINameGenerator& other);

public:
	~UINameGenerator();
	UINameGenerator();
    bool operator==(const UINameGenerator& other) const noexcept;
    unique_ptr<UINameGenerator> Clone() const;

    //?!? �� �Լ� ������ ����ϴ� Ŭ������ ���� ó���ϸ� �� �� ����.
    string MakeRegionOf(const string& region) noexcept;
    bool RemoveRegion(const string& region) noexcept;
    bool IsUnusedRegion(string_view region) noexcept;

    pair<string, string> MakeNameOf(const string& name, const string& region, ComponentID componentID,
        bool forceUniqueRegion = false) noexcept;
    bool RemoveName(const string& region, const string& name) noexcept;
    bool IsUnusedName(string_view region, string_view name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    ComponentNameGenerator* GetComponentNameGen(string_view region) const noexcept;

    unordered_svmap<string, unique_ptr<AutoNamer>> m_regionGens; //Generator �ؼ� Ű ���� ������. �� Ű���� regionŰ ���� �ٸ�. ����� base name ���� Ű���� �����ϴ� ��.
    unordered_svmap<string, unique_ptr<ComponentNameGenerator>> m_componentNameGens;
};
