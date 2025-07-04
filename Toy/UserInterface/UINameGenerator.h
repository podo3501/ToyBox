#pragma once
class JsonOperation;

class AutoNamer
{
public:
    ~AutoNamer();
    AutoNamer();
    bool operator==(const AutoNamer& other) const noexcept;

	string Generate() noexcept;
    [[nodiscard]] 
    pair<bool, bool> Recycle(int id) noexcept;
    inline bool IsDeletable() noexcept { return (m_nextID <= m_recycled.size()); }

    void SerializeIO(JsonOperation& operation);

private:
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
    string Create(const string& prefix, ComponentID id) noexcept;
    bool Remove(const string& name) noexcept;

    void SerializeIO(JsonOperation& operation);
    
private:
    map<ComponentID, AutoNamer> m_namers;
};

class UINameGenerator
{
public:
	~UINameGenerator();
	UINameGenerator();
    bool operator==(const UINameGenerator& other) const noexcept;

    string MakeRegionOf(const string& region) noexcept;
    bool TryRemoveRegion(const string& region) noexcept;

	string MakeNameOf(const string& name, const string& region, ComponentID componentID) noexcept;
    bool TryRemoveName(const string& region, const string& name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    struct RegionAutoNamer
    {
        RegionAutoNamer() = default;
        explicit RegionAutoNamer(bool isBaseName) noexcept
            : deleted(!isBaseName)
        {}

        AutoNamer namer;
        bool deleted{ false };
    };

    bool TryMarkRegionDeleted(unordered_map<string, RegionAutoNamer>::iterator iter) noexcept;

    unordered_map<string, RegionAutoNamer> m_regionNameGens;
    unordered_map<string, ComponentNameGenerator> m_componentNameGens;
};
