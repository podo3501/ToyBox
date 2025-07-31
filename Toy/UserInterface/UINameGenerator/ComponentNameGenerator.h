#pragma once
#include "AutoNamer.h"
#include "Utils/stl_ext.hpp"

class JsonOperation;
class ComponentNameGenerator
{
public:
    ~ComponentNameGenerator();
    ComponentNameGenerator();
    ComponentNameGenerator(const ComponentNameGenerator& other);
    ComponentNameGenerator& operator=(const ComponentNameGenerator& other);

    bool operator==(const ComponentNameGenerator& other) const noexcept;
    string MakeNameFromComponent(const string& name) noexcept;
    string MakeNameFromBase(const string& name) noexcept;
    bool Remove(const string& name) noexcept;
    bool IsUniqueName(string_view name) const noexcept;
    void SerializeIO(JsonOperation& operation);

private:
    //unordered_svmap<string, AutoNamer> m_namers;
    unordered_svmap<string, unique_ptr<AutoNamer>> m_namers;
};