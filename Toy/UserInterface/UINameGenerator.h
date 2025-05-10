#pragma once

class AutoNamer
{
public:
	string Generate() noexcept;
    void Recycle(int id) noexcept;

private:
    int m_nextID{ 0 };
    set<int> m_recycled;
};

enum class ComponentID;
class ComponentNameGenerator
{
public:
    string Create(ComponentID componentID) noexcept;
    bool Remove(const string& name) noexcept;
    
private:
    map<ComponentID, AutoNamer> m_namers;
};

class UINameGenerator
{
public:
	~UINameGenerator() = default;
	UINameGenerator() = default;

	string MakeNameOf(const string& region, ComponentID componentID) noexcept;
    bool RemoveNameOf(const string& region, const string& name) noexcept;

private:
    unordered_map<string, ComponentNameGenerator> m_regionNames;
};
