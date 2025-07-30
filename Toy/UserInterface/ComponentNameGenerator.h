#pragma once
#include "AutoNamer.h"

class JsonOperation;
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