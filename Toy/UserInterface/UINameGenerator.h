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

    //?!? 두 함수 패턴이 비슷하니 클래스를 만들어서 처리하면 될 것 같다.
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
        bool deleted{ false }; //base 이름으로 생성했을때에는 base 이름을 지우면 이것이 true가 된다. 자동생성할때에는 base 이름은 자동생성되지 않는다. 베이스 이름과 자동생성된 이름이 다 삭제가 되면 이 클래스도 삭제한다.
    };

    bool TryMarkRegionDeleted(unordered_map<string, RegionAutoNamer>::iterator iter) noexcept;

    unordered_svmap<string, RegionAutoNamer> m_regionNameGens;
    unordered_svmap<string, ComponentNameGenerator> m_componentNameGens;
};
