#pragma once

class UIComponent;
enum class Origin;
class TransformComponent;

template <typename T>
class JsonNavigator {
public:
	// 초기 JSON 객체 설정 (unique_ptr로 관리)
	explicit JsonNavigator(std::unique_ptr<T> root)
		: root(std::move(root)), current(this->root.get()) {}

	// 특정 키로 내려가기 (키가 없으면 생성)
	bool goToKey(const std::string& key, bool array = false) 
	{
		if (!current->contains(key)) 
		{
			if (array)
				(*current)[key] = T::array();
			else
				(*current)[key] = T();  // 키가 없으면 빈 객체로 초기화
		}

		parentStack.push(current);           // 부모 객체를 스택에 저장
		current = &(*current)[key];          // 현재 객체를 하위 객체로 변경
		return true;
	}

	// 부모 객체로 올라가기
	bool goBack() {
		if (!parentStack.empty()) {
			current = parentStack.top();     // 부모 객체로 돌아가기
			parentStack.pop();               // 스택에서 부모 객체 제거
			return true;
		}
		return false;
	}

	// 현재 객체 값 가져오기
	T& getCurrent() {
		return *current;
	}

	//json을 읽을때 특정 json으로 읽어야 할 때가 있다. 그때 살짝 자리바꿈 한 다음에 goBack으로 돌려놓는다.
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

	//// 현재 객체 값 설정
	//template <typename ValueType>
	//void setValue(const ValueType& value) {
	//    *current = value;
	//}

	//// 특정 키에 값 설정
	//template <typename ValueType>
	//void SetKeyValue(const std::string& key, const ValueType& value) {
	//	(*current)[key] = value;
	//}

	// 루트 JSON 반환 (상태 확인용)
	const T& getRoot() const {
		return *root;
	}

private:
	std::unique_ptr<T> root;         // 루트 객체
	T* current;                      // 현재 탐색 중인 객체
	std::stack<T*> parentStack;      // 부모 객체 추적을 위한 스택
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

