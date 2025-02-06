#pragma once

class UIComponent;

//has로 리팩토링이 안되서 crtp로 리팩토링함.
template<typename T>
class UIContainer
{
public:
	~UIContainer() = default;
	UIContainer() = default;

	UIComponent* GetRoot() noexcept;
	UIComponent* GetRegionRoot() noexcept;
	UIComponent* GetParentRegionRoot() noexcept;

	//children을 순회하는 함수 모음
	void ForEachChild(function<void(UIComponent*)> func) noexcept;
	void ForEachChildBool(function<bool(UIComponent*)> func) noexcept;
	void ForEachChildConst(function<void(const UIComponent*)> func) const noexcept;
	void ForEachChildBFS(std::function<void(UIComponent*)> func) noexcept;

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