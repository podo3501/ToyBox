#pragma once
class JsonOperation;

class AutoNamer
{
public:
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
    bool operator==(const ComponentNameGenerator& other) const noexcept;
    string Create(ComponentID componentID) noexcept;
    bool Remove(const string& name) noexcept;

    void SerializeIO(JsonOperation& operation);
    
private:
    map<ComponentID, AutoNamer> m_namers;
};

class UINameGenerator
{
public:
	~UINameGenerator() = default;
	UINameGenerator() = default;
    bool operator==(const UINameGenerator& other) const noexcept;

	string MakeNameOf(const string& region, ComponentID componentID) noexcept;
    bool RemoveNameOf(const string& region, const string& name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    unordered_map<string, ComponentNameGenerator> m_regionNames;
};
