#pragma once

template<typename T> requires is_default_constructible_v<T>&& is_copy_assignable_v<T>
class Property;
class UIComponent;
class Scene;
class UITransform;
enum class Origin;
enum class ButtonState; //enum�� ������ �߰��� �ϸ� �ȴ�.

namespace Tool
{
	enum class ResolutionType;
}

template <typename T>
class JsonNavigator {
public:
	// �ʱ� JSON ��ü ���� (unique_ptr�� ����)
	explicit JsonNavigator(unique_ptr<T> root)
		: root(move(root)), current(this->root.get()) {}

	// Ư�� Ű�� �������� (Ű�� ������ ����)
	bool GotoKey(const string& key, bool createArray = false)
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
	unique_ptr<T> root;         // ��Ʈ ��ü
	T* current;                      // ���� Ž�� ���� ��ü
	std::stack<T*> parentStack;      // �θ� ��ü ������ ���� ����
};

// STL �����̳����� Ȯ���ϴ� ��Ÿ�Լ�
template <typename T>
struct is_stl_container : false_type {};

// STL �����̳� Ư��ȭ
template <typename... Args>
struct is_stl_container<vector<Args...>> : true_type {};

template <typename... Args>
struct is_stl_container<list<Args...>> : true_type {};

template <typename... Args>
struct is_stl_container<deque<Args...>> : true_type {};

template<typename T>
concept IsClassType = is_class_v<T>;

template<typename T>	//�⺻ Ÿ�� �� enum Ÿ��
concept IsBasicType =
is_arithmetic_v<T> ||
is_same_v<T, string> ||
is_same_v<T, size_t> ||
is_same_v < T, ButtonState> ||
is_same_v<T, Tool::ResolutionType>;

template<typename T>
concept IsBasicContainer =
is_stl_container<T>::value &&
!IsClassType<typename T::value_type>;

template<typename T>
concept IsClassContainer =
is_stl_container<T>::value &&
IsClassType<typename T::value_type>;

template<typename T>
concept IsClass =
!IsBasicType<T> &&
!is_stl_container<T>::value &&
!requires {                     // Map�� ���� ���� �����̳� ����
	typename T::key_type;
	typename T::mapped_type;
} &&
IsClassType<T>;

//Json�� stl�� � ���� �����ϴ��� �˻��ϰ� ������ �ȴٸ� stl �����̳� Ư��ȭ�� �߰��ؼ� 
//�⺻ Process�� ����ؼ� �Ǵ��� ���� Ȯ����, ������ �ȵȴٸ� Process�Լ��� �߰�����.
template<typename T>
concept Available =
IsBasicType<T> || IsBasicContainer<T>;

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
	template<IsClass T>
	void Process(const string& key, T& data) noexcept;
	template<IsNotUIComponent T>
	void Process(const string& key, unique_ptr<T>& data);
	template<IsClassContainer T>
	void Process(const string& key, T& data) noexcept;
	template<typename T>
	void Process(const string& key, vector<unique_ptr<T>>& data);
	template<typename T>
	void Process(const string& key, Property<T>& data);

	void Process(const string& key, XMINT2& data) noexcept;
	void Process(const string& key, XMUINT2& data) noexcept;
	void Process(const string& key, Rectangle& data) noexcept;
	void Process(const string& key, Origin& data) noexcept; //�⺻ ���ø��� �־ �ǳ�, �����Ҷ� string���� �����ϸ� ���Ⱑ ����.
	void Process(const string& key, Vector2& data) noexcept;
	void Process(const string& key, unique_ptr<UIComponent>& data);
	void Process(const string& key, vector<unique_ptr<UIComponent>>& datas);
	void Process(const string& key, wstring& data) noexcept;
	void Process(const string& key, map<wstring, wstring>& data) noexcept;
	void Process(const string& key, map<int, UITransform>& datas) noexcept;
	void Process(const string& key, deque<wstring>& data) noexcept;
	void Write(const string& key, UIComponent* data);

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
	void UpdateJson(UIComponent* data) noexcept;
	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName);
	void ProcessImpl(const string& key, auto readFunc, auto writeFunc);

	unique_ptr<JsonNavigator<nlohmann::ordered_json>> m_write;
	unique_ptr<JsonNavigator<nlohmann::json>> m_read;
};

#include "JsonOperation.hpp"

