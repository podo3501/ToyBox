#pragma once
#include "../stl_ext.hpp"

class JsonOperation;

class AutoNamer //�̸��� base, base_1... ��, base_0 �̶�� �ڵ� �̸��� ����.
{
public:
    ~AutoNamer();
    AutoNamer();
    bool operator==(const AutoNamer& other) const noexcept;

	string Generate() noexcept;
    [[nodiscard]] pair<bool, bool> Recycle(int id) noexcept;
    inline bool IsDeletable() noexcept { return (m_nextID <= m_recycled.size()); }
    bool IsUsed(int id) const noexcept;
    void SerializeIO(JsonOperation& operation);

private:
    inline bool HasBeenGenerated(int id) const noexcept { return id >= 0 && id < m_nextID; }

    int m_nextID{ 0 };
    set<int> m_recycled;
    bool m_isDeletable{ false };
};

//autonamer�ȿ�
//bool deleted{ false }; //base �̸����� �������������� base �̸��� ����� �̰��� true�� �ȴ�. �ڵ������Ҷ����� base �̸��� �ڵ��������� �ʴ´�. ���̽� �̸��� �ڵ������� �̸��� �� ������ �Ǹ� �� Ŭ������ �����Ѵ�.
//�̰� �־ ���������� base �̸��� ���� å���� ������ �����. �̰��� ���� ��Ʈ���ĸ� ����� �ߴµ�, �ܼ��� Ŭ���� �ߺ��̶� �Ұ� �ϳ��� �ִ°� �� ���� �Ǵ� ����.
//���� m_nextID���� -1���� �����ϴ� �ɷ� �ϰ� base�� ���⼭ �ϰ������� ó���ϰԲ� �ٲٴ°� ������ ����.

enum class ComponentID;
class ComponentNameGenerator
{
public:
    ~ComponentNameGenerator();
    ComponentNameGenerator();

    bool operator==(const ComponentNameGenerator& other) const noexcept;
    string MakeRegion() noexcept;
    string MakeNameFromComponent(const string& name) noexcept;
    string MakeNameFromBase(const string& name) noexcept;

    pair<bool, bool> RemoveRegion(int id) noexcept;
    bool Remove(const string& name) noexcept;
    bool IsUniqueName(string_view name) const noexcept;

    void SerializeIO(JsonOperation& operation);
    
private:
    AutoNamer m_region;
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
    bool RemoveRegion(const string& region) noexcept;
    bool IsUniqueRegion(string_view region) noexcept;

	string MakeNameOf(const string& name, const string& region, ComponentID componentID) noexcept;
    bool RemoveName(const string& region, const string& name) noexcept;
    bool IsUniqueName(string_view region, string_view name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    unordered_svmap<string, ComponentNameGenerator> m_componentNameGens;
};
