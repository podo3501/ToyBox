#pragma once
#include "../stl_ext.hpp"

class JsonOperation;

class AutoNamer
{
public:
    ~AutoNamer();
    AutoNamer();
    bool operator==(const AutoNamer& other) const noexcept;

	string Generate() noexcept;
    [[nodiscard]] pair<bool, bool> Recycle(int id) noexcept;
    inline bool IsDeletable() noexcept { return (m_nextID <= m_recycled.size()); }
    bool IsUnused(int id) const noexcept;
    void SerializeIO(JsonOperation& operation);

private:
    inline bool HasBeenGenerated(int id) const noexcept { return id >= 0 && id < m_nextID; }

    int m_nextID{ 0 };
    set<int> m_recycled;
    bool m_isDeletable{ false };
};

enum class ComponentID;
class ComponentNameGenerator
{
public:
    ~ComponentNameGenerator();
    ComponentNameGenerator();

    bool operator==(const ComponentNameGenerator& other) const noexcept;
    string MakeNameFromComponent(const string& name) noexcept;
    string MakeNameFromBase(const string& name) noexcept;
    bool Remove(const string& name) noexcept;
    bool IsUniqueName(string_view name) const noexcept;
    bool Empty() const noexcept { return m_namers.empty(); }

    void SerializeIO(JsonOperation& operation);
    
private:
    unordered_svmap<string, AutoNamer> m_namers;
};

class UINameGenerator
{
public:
	~UINameGenerator();
	UINameGenerator();
    bool operator==(const UINameGenerator& other) const noexcept;

    //?!? �� �Լ� ������ ����ϴ� Ŭ������ ���� ó���ϸ� �� �� ����.
    string MakeRegionOf(const string& region) noexcept;
    bool TryRemoveRegion(const string& region) noexcept;
    bool IsUniqueRegion(string_view region) noexcept;

	string MakeNameOf(const string& name, const string& region, ComponentID componentID) noexcept;
    bool TryRemoveName(const string& region, const string& name) noexcept;
    bool IsUniqueName(string_view region, string_view name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    struct RegionAutoNamer
    {
        RegionAutoNamer() = default;
        explicit RegionAutoNamer(bool isBaseName) noexcept
            : deleted(!isBaseName)
        {}

        AutoNamer namer;
        bool deleted{ false }; //base �̸����� �������������� base �̸��� ����� �̰��� true�� �ȴ�. �ڵ������Ҷ����� base �̸��� �ڵ��������� �ʴ´�. ���̽� �̸��� �ڵ������� �̸��� �� ������ �Ǹ� �� Ŭ������ �����Ѵ�.
    };

    bool TryMarkRegionDeleted(unordered_map<string, RegionAutoNamer>::iterator iter) noexcept;

    unordered_svmap<string, RegionAutoNamer> m_regionNameGens;
    unordered_svmap<string, ComponentNameGenerator> m_componentNameGens;
};
