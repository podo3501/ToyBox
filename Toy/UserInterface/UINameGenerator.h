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
    string MakeNameFromComponent(const string& name) noexcept;
    string MakeNameFromBase(const string& name) noexcept;
    bool Remove(const string& name) noexcept;
    bool IsUniqueName(string_view name) const noexcept;
    void SerializeIO(JsonOperation& operation);
    
private:
    unordered_svmap<string, AutoNamer> m_namers;
};

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
    bool IsUniqueRegion(string_view region) noexcept;

    pair<string, string> MakeNameOf(const string& name, const string& region, ComponentID componentID,
        bool forceUniqueRegion = false) noexcept;
    bool RemoveName(const string& region, const string& name) noexcept;
    bool IsUniqueName(string_view region, string_view name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    unordered_svmap<string, AutoNamer> m_regionGens; //Generator �ؼ� Ű ���� ������. �� Ű���� regionŰ ���� �ٸ�. ����� base name ���� Ű���� �����ϴ� ��.
    unordered_svmap<string, ComponentNameGenerator> m_componentNameGens;
};
