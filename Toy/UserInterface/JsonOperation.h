#pragma once

class UIComponent;
enum class Origin;
class TransformComponent;

template <typename T>
class JsonNavigator {
public:
	// �ʱ� JSON ��ü ���� (unique_ptr�� ����)
	explicit JsonNavigator(std::unique_ptr<T> root)
		: root(std::move(root)), current(this->root.get()) {}

	// Ư�� Ű�� �������� (Ű�� ������ ����)
	bool goToKey(const std::string& key, bool array = false) 
	{
		if (!current->contains(key)) 
		{
			if (array)
				(*current)[key] = T::array();
			else
				(*current)[key] = T();  // Ű�� ������ �� ��ü�� �ʱ�ȭ
		}

		parentStack.push(current);           // �θ� ��ü�� ���ÿ� ����
		current = &(*current)[key];          // ���� ��ü�� ���� ��ü�� ����
		return true;
	}

	// �θ� ��ü�� �ö󰡱�
	bool goBack() {
		if (!parentStack.empty()) {
			current = parentStack.top();     // �θ� ��ü�� ���ư���
			parentStack.pop();               // ���ÿ��� �θ� ��ü ����
			return true;
		}
		return false;
	}

	// ���� ��ü �� ��������
	T& getCurrent() {
		return *current;
	}

	//json�� ������ Ư�� json���� �о�� �� ���� �ִ�. �׶� ��¦ �ڸ��ٲ� �� ������ goBack���� �������´�.
	void setCurrent(T* _current)
	{
		current = _current;
	}

	//T& GetKeyJson(const std::string& key)
	//{
	//	return (*current)[key];
	//}

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
	const T& getRoot() const {
		return *root;
	}

private:
	std::unique_ptr<T> root;         // ��Ʈ ��ü
	T* current;                      // ���� Ž�� ���� ��ü
	std::stack<T*> parentStack;      // �θ� ��ü ������ ���� ����
};


template<typename T>
concept JsonPrimitive = is_arithmetic<T>::value || is_same_v<T, string>;

class JsonOperation
{
public:
	JsonOperation();
	virtual ~JsonOperation();

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<JsonPrimitive T>
	void Process(const string& key, T& data) noexcept
	{
		if (IsWrite())
		{
			auto& j = GetWrite();
			j[key] = data;
		}
		else
		{
			auto& j = GetRead();
			data = j[key];
		}
	}

	template<typename T>
	void Process(const string& key, unique_ptr<T>& data) noexcept
	{
		if (IsWrite())
		{
			m_write->goToKey(key);
			data->SerializeIO(this);
			m_write->goBack();
		}
		else
		{
			m_read->goToKey(key);
			data->SerializeIO(this);
			m_read->goBack();
		}
	}

	void Process(const string& key, Rectangle& data) noexcept;
	void Process(const string& key, Origin& data) noexcept;
	void Process(const string& key, Vector2& outData) noexcept;
	void Process(const string& key, vector<unique_ptr<TransformComponent>>& data);
	void Process(const string& key, unique_ptr<UIComponent>& data);

private:
	nlohmann::ordered_json& GetWrite();
	nlohmann::json& GetRead();
	void UpdateJson(const unique_ptr<UIComponent>& component, const string& type) noexcept;
	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName);

	unique_ptr<JsonNavigator<nlohmann::ordered_json>> m_write;
	unique_ptr<JsonNavigator<nlohmann::json>> m_read;
};

