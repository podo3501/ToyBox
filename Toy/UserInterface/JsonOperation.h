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


template<typename T>
concept JsonPrimitive = is_arithmetic<T>::value || is_same_v<T, string> || is_same_v<T, size_t>;

template<typename T>
concept IsNotUIComponent = !std::is_same_v<T, UIComponent>;

class JsonOperation
{
public:
	JsonOperation();
	virtual ~JsonOperation();

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<JsonPrimitive T>
	void Process(const string& key, T& data) noexcept;
	template<IsNotUIComponent T>
	void Process(const string& key, unique_ptr<T>& data);
	template<typename T>
	void Process(const string& key, vector<unique_ptr<T>>& data);

	void Process(const string& key, Rectangle& data) noexcept;
	void Process(const string& key, Origin& data) noexcept;
	void Process(const string& key, Vector2& data) noexcept;
	void Process(const string& key, unique_ptr<UIComponent>& data);
	void Process(const string& key, wstring& data) noexcept;
	void Process(const string& key, map<wstring, wstring>& data) noexcept;

private:
	nlohmann::ordered_json& GetWrite();
	nlohmann::json& GetRead();

	void UpdateJson(const unique_ptr<UIComponent>& data) noexcept;
	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName);
	void ProcessImpl(const string& key, auto readFunc, auto writeFunc);

	unique_ptr<JsonNavigator<nlohmann::ordered_json>> m_write;
	unique_ptr<JsonNavigator<nlohmann::json>> m_read;
};

//////////////////////////////////////////////////////////////////////////

template<JsonPrimitive T>
void JsonOperation::Process(const string& key, T& data) noexcept
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

template<IsNotUIComponent T>
void JsonOperation::Process(const string& key, unique_ptr<T>& data)
{
	if (IsWrite())
	{
		m_write->GotoKey(key);
		data->SerializeIO(this);
		m_write->GoBack();
	}
	else
	{
		m_read->GotoKey(key);
		auto newClass = std::make_unique<T>();
		newClass->SerializeIO(this);
		data = move(newClass);
		m_read->GoBack();
	}
}

template<typename T>
void JsonOperation::Process(const string& key, vector<unique_ptr<T>>& data)
{
	if (IsWrite())
	{
		if (data.empty())
			return;

		m_write->GotoKey(key, true);
		for (auto& comp : data)
		{
			JsonOperation jsOp{};
			comp->SerializeIO(&jsOp);
			m_write->GetCurrent().push_back(jsOp.GetWrite());
		}
		m_write->GoBack();
	}
	else
	{
		const auto& j = m_read->GetCurrent();
		if (j.contains(key) == false)
			return;

		data.clear();
		m_read->GotoKey(key);
		for (auto& compJson : m_read->GetCurrent())
		{
			m_read->SetCurrent(&compJson);
			auto comp = std::make_unique<T>();
			comp->SerializeIO(this);
			data.emplace_back(move(comp));
		}
		m_read->GoBack();
	}
}

