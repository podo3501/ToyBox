#pragma once
class JsonOperation;

class AutoNamer
{
public:
    ~AutoNamer();
    AutoNamer();
    bool operator==(const AutoNamer& other) const noexcept;

	string Generate() noexcept;
    void Recycle(int id) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    int m_nextID{ 0 };
    set<int> m_recycled;
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

	string MakeNameOf(const string& name, const string& region, ComponentID componentID) noexcept;
    bool TryRemoveName(const string& region, const string& name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    unordered_map<string, ComponentNameGenerator> m_regionNames;
};
