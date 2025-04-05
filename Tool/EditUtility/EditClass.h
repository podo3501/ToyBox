#pragma once

//�޺��ڽ��� ó���� ����(����) �Ǵ� ������ �ְ� ����Ʈ�� �������� ���õǴ� ������ �ִ�.
class EditCombo
{
public:
	EditCombo() = delete;
	~EditCombo();
	EditCombo(const string& name, const vector<string> keys) noexcept;
	void Render(function<void(const string&)> OnChangeKey);

private:
	string m_name;
	int m_index{ 0 };
	vector<string> m_keyStorage;
	vector<const char*> m_keys;
};

class EditListBox
{
public:
	EditListBox() = delete;
	~EditListBox();
	explicit EditListBox(const string& name, int maxVisibleItems) noexcept;
	void SetItems(const vector<string>& items) noexcept;
	inline void SetIndex(int index) noexcept { m_index = index; }
	void Render(function<void(int)> OnChangeItem);

private:
	void Clear() noexcept;

	string m_name;
	int m_maxVisibleItems{ 0 };
	int m_index{ -1 };
	vector<string> m_itemStorage;
	vector<const char*> m_items;
};