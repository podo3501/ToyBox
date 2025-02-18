#pragma once

class UIComponent;
namespace StateFlag { enum Type : int; }

//has로 리팩토링이 안되서 crtp로 리팩토링함.
template<typename T>
class UIHierarchy
{
public:
	~UIHierarchy() = default;
	UIHierarchy() = default;

	UIComponent* GetRoot() noexcept;
	UIComponent* GetRegionRoot() noexcept;
	UIComponent* GetParentRegionRoot() noexcept;

	//children을 순회하는 함수 모음
	void ForEachChild(function<void(UIComponent*)> func) noexcept; //기본 DFS
	bool ForEachChildUntilFail(function<bool(UIComponent*)> func) noexcept; //리턴값을 반환하는 재귀(로딩함수같은)
	void ForEachChildBool(function<bool(UIComponent*)> func) noexcept; //무언가를 찾았으면 bool 반환으로 그만 하라는 함수
	void ForEachChildConst(function<void(const UIComponent*)> func) const noexcept; //읽기전용
	void ForEachChildBFS(StateFlag::Type flag, function<void(UIComponent*)> func) noexcept; //BFS용으로 만듦. 기본은DFS

	//이름 및 Region에 관련된 함수들. 함수가 길어서 여기로 일단 대피시킴
	bool IsUniqueRegion(const string& name) noexcept;
	bool IsUniqueName(const string& name, UIComponent* self) noexcept;
	bool IsUniqueRegionName(const string& name, UIComponent* self) noexcept;
	string CreateNewName(UIComponent* attaching) noexcept;
	string CreateNewRegionName(UIComponent* attaching) noexcept;
	void GenerateUniqueName(UIComponent* attachingBlock) noexcept;
	void GenerateUniqueRegionName(UIComponent* attachingBlock) noexcept;

private:
	UIComponent* GetThis() noexcept;

protected:
	UIComponent* m_parent{ nullptr };
	vector<unique_ptr<T>> m_children;
};