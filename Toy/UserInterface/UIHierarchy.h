#pragma once

class UIComponent;
namespace StateFlag { enum Type : int; }

//has�� �����丵�� �ȵǼ� crtp�� �����丵��.
template<typename T>
class UIHierarchy
{
public:
	~UIHierarchy() = default;
	UIHierarchy() = default;

	UIComponent* GetRoot() noexcept;
	UIComponent* GetRegionRoot() noexcept;
	UIComponent* GetParentRegionRoot() noexcept;

	//children�� ��ȸ�ϴ� �Լ� ����
	void ForEachChild(function<void(UIComponent*)> func) noexcept; //�⺻ DFS
	bool ForEachChildUntilFail(function<bool(UIComponent*)> func) noexcept; //���ϰ��� ��ȯ�ϴ� ���(�ε��Լ�����)
	void ForEachChildBool(function<bool(UIComponent*)> func) noexcept; //���𰡸� ã������ bool ��ȯ���� �׸� �϶�� �Լ�
	void ForEachChildConst(function<void(const UIComponent*)> func) const noexcept; //�б�����
	void ForEachChildBFS(StateFlag::Type flag, function<void(UIComponent*)> func) noexcept; //BFS������ ����. �⺻��DFS

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