#pragma once

//class CException
//{
//public:
//    CException() = default;
//    CException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);
//
//    std::wstring ToString()const;
//
//    HRESULT ErrorCode = S_OK;
//    std::wstring FunctionName;
//    std::wstring Filename;
//    int LineNumber = -1;
//};

#ifndef ReturnIfFalse
#define ReturnIfFalse(x)                       \
{                                                             \
    if(!(x)) return false;                          \
}
#endif

#ifndef ReturnIfFailed
#define ReturnIfFailed(x)                                              \
{                                                                     \
    if(FAILED(x))                                            \
        return false;                                                \
}
#endif

#ifndef ReturnIfNullptr
#define ReturnIfNullptr(x)                                              \
{                                                                     \
    if(x == nullptr)                                            \
        return false;                                                \
}
#endif

wstring StringToWString(const string& str);
string RemoveNullTerminator(const string& str);
string WStringToString(const wstring& wstr);

//스마트 포인터 비교
template <typename T>
bool CompareUniquePtr(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs)
{
	if (lhs == nullptr && rhs == nullptr)
		return true;

	if (lhs == nullptr || rhs == nullptr)
		return false;

	auto result{ *lhs == *rhs }; // 가리키는 값 비교
	assert(result);

	return result;
}

//시퀀스 컨테이너 비교
template <typename T>
bool CompareSeq(const vector<unique_ptr<T>>& lhs, const vector<unique_ptr<T>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	return equal(lhs.begin(), lhs.end(), rhs.begin(), CompareUniquePtr<T>);
}

//원소가 몇번째에 있는지 확인
template <typename Container, typename T>
std::optional<int> FindIndex(const Container& container, const T& target)
{
	auto it = std::ranges::find(container, target);
	if (it != container.end())
		return static_cast<int>(std::distance(container.begin(), it));
	return std::nullopt;
}

//연관 컨테이너 비교
template <typename Key, typename Value>
bool CompareAssoc(const map<Key, unique_ptr<Value>>& lhs, const map<Key, unique_ptr<Value>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	auto it1 = lhs.begin();
	auto it2 = rhs.begin();

	while (it1 != lhs.end() && it2 != rhs.end()) {
		// 키 비교
		if (it1->first != it2->first)
			return false;

		// 값 비교
		if (!CompareUniquePtr(it1->second, it2->second))
			return false;

		++it1;
		++it2;
	}

	return true;
}

inline bool CompareEpsilon(float a, float b, float epsilon = 1e-6f) 
{
	return fabs(a - b) <= epsilon;
}

inline bool CompareEpsilon(const Vector2& a, const Vector2& b, float epsilon = 1e-6f)
{
	return CompareEpsilon(a.x, b.x, epsilon) && CompareEpsilon(a.y, b.y, epsilon);
}

inline XMINT2 operator+(const XMINT2& a, const XMINT2& b) { return XMINT2(a.x + b.x, a.y + b.y); }
inline XMINT2 operator-(const XMINT2& a, const XMINT2& b) { return XMINT2(a.x - b.x, a.y - b.y); }
inline XMINT2 operator*(const XMINT2& a, const XMINT2& b) { return XMINT2(a.x * b.x, a.y * b.y); }
inline XMINT2 operator/(const XMINT2& a, const XMINT2& b) { 
	return XMINT2((b.x != 0) ? a.x / b.x : 0, (b.y != 0) ? a.y / b.y : 0); }

inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x - b.x, a.y - b.y); }

inline XMINT2 Vector2ToXMINT2(const Vector2& rhs) {
	return { static_cast<int32_t>(rhs.x), static_cast<int32_t>(rhs.y) };
}
inline Vector2 XMINT2ToVector2(const XMINT2& rhs) {
	return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
}
inline XMINT2 ImVec2ToXMINT2(const ImVec2& rhs) {
	return { static_cast<int32_t>(rhs.x), static_cast<int32_t>(rhs.y) };
}
inline ImVec2 XMINT2ToImVec2(const XMINT2& rhs) {
	return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
}

inline bool operator==(const RECT& lhs, const RECT& rhs) {
	return lhs.left == rhs.left &&
		lhs.top == rhs.top &&
		lhs.right == rhs.right &&
		lhs.bottom == rhs.bottom;
}

inline bool operator!=(const RECT& lhs, const RECT& rhs) {
	return !(lhs == rhs);
}