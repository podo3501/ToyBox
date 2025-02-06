#pragma once

class UIComponent;

//has�� �����丵�� �ȵǼ� crtp�� �����丵��.
template<typename T>
class UIContainer
{
public:
	~UIContainer() = default;
	UIContainer() = default;

	UIComponent* GetRoot() noexcept;
	UIComponent* GetRegionRoot() noexcept;
	UIComponent* GetParentRegionRoot() noexcept;

	//children�� ��ȸ�ϴ� �Լ� ����
	void ForEachChild(function<void(UIComponent*)> func) noexcept;
	void ForEachChildBool(function<bool(UIComponent*)> func) noexcept;
	void ForEachChildConst(function<void(const UIComponent*)> func) const noexcept;
	void ForEachChildBFS(std::function<void(UIComponent*)> func) noexcept;

	//�̸� �� Region�� ���õ� �Լ���. �Լ��� �� ����� �ϴ� ���ǽ�Ŵ
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