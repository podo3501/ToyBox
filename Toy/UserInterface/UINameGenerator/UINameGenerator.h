#pragma once
#include "Shared/Utils/StlTypeExt.hpp"

class JsonOperation;
class AutoNamer;
class ComponentNameGenerator;
enum class ComponentID;
class UINameGenerator
{
protected:
    UINameGenerator(const UINameGenerator& other);

public:
	~UINameGenerator();
	UINameGenerator();
    bool operator==(const UINameGenerator& other) const noexcept;
    unique_ptr<UINameGenerator> Clone() const;

    //?!? 두 함수 패턴이 비슷하니 클래스를 만들어서 처리하면 될 것 같다.
    string MakeRegionOf(const string& region) noexcept;
    bool RemoveRegion(const string& region) noexcept;
    bool IsUnusedRegion(string_view region) noexcept;

    pair<string, string> MakeNameOf(const string& name, const string& region, ComponentID componentID,
        bool forceUniqueRegion = false) noexcept;
    bool RemoveName(const string& region, const string& name) noexcept;
    bool IsUnusedName(string_view region, string_view name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    ComponentNameGenerator* GetComponentNameGen(string_view region) const noexcept;

    unordered_svmap<string, unique_ptr<AutoNamer>> m_regionGens; //Generator 해서 키 값을 생성함. 이 키값과 region키 값이 다름. 여기는 base name 으로 키값을 생성하는 곳.
    unordered_svmap<string, unique_ptr<ComponentNameGenerator>> m_componentNameGens;
};
