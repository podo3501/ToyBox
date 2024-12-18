#pragma once

class UIComponent;
class Scene;
enum class Origin;
enum class ButtonState;

namespace Tool
{
	enum class ResolutionType;
}

template <typename T>
class JsonNavigator {
public:
	// �ʱ� JSON ��ü ���� (unique_ptr�� ����)
	explicit JsonNavigator(std::unique_ptr<T> root)
		: root(std::move(root)), current(this->root.get()) {}

	// Ư�� Ű�� �������� (Ű�� ������ ����)
	bool GotoKey(const std::string& key, bool createArray = false)
	{
		if (!current || key.empty()) return false;

		if (!current->contains(key)) {
			(*current)[key] = createArray ? T::array() : T();
		}

		parentStack.push(current);
		current = &(*current)[key];
		return true;
	}

	// �θ� ��ü�� �ö󰡱�
	bool GoBack() {
		if (!parentStack.empty()) {
			current = parentStack.top();     // �θ� ��ü�� ���ư���
			parentStack.pop();               // ���ÿ��� �θ� ��ü ����
			return true;
		}
		return false;
	}

	// ���� ��ü �� ��������
	T& GetCurrent() {
		return *current;
	}

	//json�� ������ Ư�� json���� �о�� �� ���� �ִ�. �׶� ��¦ �ڸ��ٲ� �� ������ goBack���� �������´�.
	void SetCurrent(T* _current)
	{
		current = _current;
	}

	bool IsEmpty()
	{
		return current->empty();
	}

	//// ���� ��ü �� ����
	//template <typename ValueType>
	//void setValue(const ValueType& value) {
	//    *current = value;
	//}

	//// Ư�� Ű�� �� ����
	//template <typename ValueType>
	//void SetKeyValue(const std::string& key, const ValueType& value) {
	//	(*current)[key] = value;
	//}

	// ��Ʈ JSON ��ȯ (���� Ȯ�ο�)
	const T& GetRoot() const {
		return *root;
	}

private:
	std::unique_ptr<T> root;         // ��Ʈ ��ü
	T* current;                      // ���� Ž�� ���� ��ü
	std::stack<T*> parentStack;      // �θ� ��ü ������ ���� ����
};

// STL �����̳����� Ȯ���ϴ� ��Ÿ�Լ�
template <typename T>
struct is_stl_container : std::false_type {};

// STL �����̳� Ư��ȭ
template <typename... Args>
struct is_stl_container<std::vector<Args...>> : std::true_type {};

template <typename... Args>
struct is_stl_container<std::list<Args...>> : std::true_type {};

template <typename... Args>
struct is_stl_container<std::deque<Args...>> : std::true_type {};

template<typename T>
concept IsClassContainer =
	is_stl_container<T>::value &&
	std::is_class_v<typename T::value_type>;

//Json�� stl�� � ���� �����ϴ��� �˻��ϰ� ������ �ȴٸ� stl �����̳� Ư��ȭ�� �߰��ؼ� 
//�⺻ Process�� ����ؼ� �Ǵ��� ���� Ȯ����, ������ �ȵȴٸ� Process�Լ��� �߰�����.
template<typename T>
concept Available = 
	is_arithmetic<T>::value ||
	is_same_v<T, string> ||
	is_same_v<T, size_t> ||
	is_same_v<T, Tool::ResolutionType> ||
	is_stl_container<T>::value &&	// stl �����̳ʰ� �⺻(int����)���϶� �׸��� 
	!std::is_class_v<typename T::value_type>;	//�� �����Ͱ� Ŭ������ ��Ʈ���İ� �ƴѰ��

template<typename T>
concept IsNotUIComponent = !std::is_same_v<T, UIComponent>;

class JsonOperation
{
public:
	JsonOperation();
	JsonOperation(const nlohmann::json& read);
	virtual ~JsonOperation();

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<Available T>
	void Process(const string& key, T& data) noexcept;
	template<IsNotUIComponent T>
	void Process(const string& key, unique_ptr<T>& data);
	template<IsClassContainer T>
	void Process(const string& key, T& data) noexcept;
	template<typename T>
	void Process(const string& key, vector<unique_ptr<T>>& data);

	void Process(const string& key, Rectangle& data) noexcept;
	void Process(const string& key, Origin& data) noexcept;
	void Process(const string& key, Vector2& data) noexcept;
	void Process(const string& key, unique_ptr<UIComponent>& data);
	void Process(const string& key, wstring& data) noexcept;
	void Process(const string& key, map<wstring, wstring>& data) noexcept;
	void Process(const string& key, deque<wstring>& data) noexcept;

private:
	template<typename T>
	unique_ptr<T> CreateData(const nlohmann::json& readJ = nullptr);
	template <typename ProcessFunc>
	void ProcessWriteKey(const string& key, ProcessFunc processFunc);
	template <typename ProcessFunc>
	void ProcessReadKey(const string& key, ProcessFunc processFunc);

	nlohmann::ordered_json& GetWrite();
	nlohmann::json& GetRead();

	void UpdateJson(const unique_ptr<UIComponent>& data) noexcept;
	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName);
	void ProcessImpl(const string& key, auto readFunc, auto writeFunc);

	unique_ptr<JsonNavigator<nlohmann::ordered_json>> m_write;
	unique_ptr<JsonNavigator<nlohmann::json>> m_read;
};

#include "JsonOperation.hpp"

