#pragma once
#include "../stl_ext.hpp"

class JsonOperation;

class AutoNamer //이름은 base, base_1... 즉, base_0 이라는 자동 이름은 없다.
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

//autonamer안에
//bool deleted{ false }; //base 이름으로 생성했을때에는 base 이름을 지우면 이것이 true가 된다. 자동생성할때에는 base 이름은 자동생성되지 않는다. 베이스 이름과 자동생성된 이름이 다 삭제가 되면 이 클래스도 삭제한다.
//이걸 넣어서 내부적으로 base 이름에 대한 책임을 가지게 만든다. 이것을 위한 스트럭쳐를 만들까 했는데, 단순히 클래스 중복이라 불값 하나를 넣는게 더 옳은 판단 같다.
//차라리 m_nextID값을 -1부터 시작하는 걸로 하고 base를 여기서 일괄적으로 처리하게끔 바꾸는게 좋을것 같다.

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

    //?!? 두 함수 패턴이 비슷하니 클래스를 만들어서 처리하면 될 것 같다.
    string MakeRegionOf(const string& region) noexcept;
    bool RemoveRegion(const string& region) noexcept;
    bool IsUniqueRegion(string_view region) noexcept;

    pair<string, string> MakeNameOf(const string& name, const string& region, ComponentID componentID,
        bool forceUniqueRegion = false) noexcept;
    bool RemoveName(const string& region, const string& name) noexcept;
    bool IsUniqueName(string_view region, string_view name) noexcept;

    void SerializeIO(JsonOperation& operation);

private:
    unordered_svmap<string, AutoNamer> m_regionGens; //Generator 해서 키 값을 생성함. 이 키값과 region키 값이 다름. 여기는 base name 으로 키값을 생성하는 곳.
    unordered_svmap<string, ComponentNameGenerator> m_componentNameGens;
};
