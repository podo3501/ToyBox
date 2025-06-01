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
	do{													\
		if(!(x)) return false;						\
	} while (0)
#endif

#ifndef ReturnIfFailed
#define ReturnIfFailed(x)                                              \
	do{													\
		if(FAILED(x))                                            \
			return false;                                                \
	} while (0)
#endif

void TracyStartupProfiler();
void TracyShutdownProfiler();

wstring StringToWString(const string& str) noexcept;
//string RemoveNullTerminator(const string& str) noexcept;
string WStringToString(const wstring& wstr) noexcept;
void StringToChar(const string& str, span<char> outstr) noexcept;
wstring CharToWString(const span<char> str) noexcept;
void WStringToChar(const wstring& wstr, span<char> outstr) noexcept;
wstring IntToWString(int value) noexcept;

//스마트 포인터 비교
template <typename T>
bool CompareUniquePtr(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs)
{
	if (lhs == nullptr && rhs == nullptr)
		return true;

	if (lhs == nullptr || rhs == nullptr)
		return false;

	auto result{ *lhs == *rhs }; // 가리키는 값 비교
	Assert(result);

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
optional<int> FindIndex(const Container& container, const T& target)
{
	auto it = ranges::find(container, target);
	if (it != container.end())
		return static_cast<int>(distance(container.begin(), it));
	return nullopt;
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

inline bool CompareEpsilon(float a, float b, float epsilon = 1e-6f) noexcept
{
	return fabs(a - b) <= epsilon;
}

inline bool CompareEpsilon(const Vector2& a, const Vector2& b, float epsilon = 1e-6f) noexcept
{
	return CompareEpsilon(a.x, b.x, epsilon) && CompareEpsilon(a.y, b.y, epsilon);
}

inline XMINT2 operator+(const XMINT2& a, const XMINT2& b) noexcept { return XMINT2(a.x + b.x, a.y + b.y); }
inline XMINT2 operator-(const XMINT2& a, const XMINT2& b) noexcept { return XMINT2(a.x - b.x, a.y - b.y); }
inline XMINT2 operator*(const XMINT2& a, const XMINT2& b) noexcept { return XMINT2(a.x * b.x, a.y * b.y); }
inline XMINT2 operator/(const XMINT2& a, const XMINT2& b) noexcept {
	return XMINT2((b.x != 0) ? a.x / b.x : 0, (b.y != 0) ? a.y / b.y : 0); }

inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) noexcept { return ImVec2(a.x + b.x, a.y + b.y); }
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) noexcept { return ImVec2(a.x - b.x, a.y - b.y); }

inline XMINT2 Vector2ToXMINT2(const Vector2& rhs) noexcept {
	return { static_cast<int32_t>(rhs.x), static_cast<int32_t>(rhs.y) };
}
inline Vector2 XMINT2ToVector2(const XMINT2& rhs) noexcept {
	return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
}
inline XMINT2 ImVec2ToXMINT2(const ImVec2& rhs) noexcept {
	return { static_cast<int32_t>(rhs.x), static_cast<int32_t>(rhs.y) };
}
inline ImVec2 XMINT2ToImVec2(const XMINT2& rhs) noexcept {
	return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
}

inline XMINT2 XMUINT2ToXMINT2(const XMUINT2& uint2) noexcept { return XMINT2{ static_cast<int32_t>(uint2.x), static_cast<int32_t>(uint2.y) }; };
// XMUINT2에 대한 사칙연산 오버로딩
inline XMUINT2 operator+(const XMUINT2& lhs, const XMUINT2& rhs) noexcept { return XMUINT2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline XMUINT2 operator-(const XMUINT2& lhs, const XMUINT2& rhs) noexcept { return XMUINT2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline XMUINT2 operator*(const XMUINT2& lhs, uint32_t rhs) noexcept { return XMUINT2(lhs.x * rhs, lhs.y * rhs); }
inline XMUINT2 operator/(const XMUINT2& lhs, uint32_t rhs) noexcept { return XMUINT2(lhs.x / rhs, lhs.y / rhs); }

inline bool operator==(const RECT& lhs, const RECT& rhs) {
	return lhs.left == rhs.left &&
		lhs.top == rhs.top &&
		lhs.right == rhs.right &&
		lhs.bottom == rhs.bottom;
}

inline bool operator!=(const RECT& lhs, const RECT& rhs) {
	return !(lhs == rhs);
}

inline Rectangle XMUINT2ToRectangle(const XMUINT2& size) {
	return Rectangle{
		0, 0,
		static_cast<int>(size.x),
		static_cast<int>(size.y),
	};
}

inline XMUINT2 GetSizeFromRectangle(const Rectangle& rect) { return XMUINT2(rect.width, rect.height); }

inline bool Contains(const Rectangle& rect, const XMINT2& pos) noexcept
{
	return rect.Contains(static_cast<long>(pos.x), static_cast<long>(pos.y));
}

inline XMUINT2 Union(const XMUINT2& a, const XMUINT2& b) noexcept
{
	return XMUINT2(max(a.x, b.x), max(a.y, b.y));
}

inline RECT RectangleToRect(const Rectangle& rect) noexcept
{
	RECT rct;
	rct.left = rect.x;
	rct.top = rect.y;
	rct.right = rect.x + rect.width;
	rct.bottom = rect.y + rect.height;
	return rct;
}

inline vector<RECT> RectanglesToRects(const vector<Rectangle>& rectangles) noexcept
{
	vector<RECT> rects(rectangles.size());
	ranges::transform(rectangles, rects.begin(), RectangleToRect);
	return rects;
}

Rectangle CombineRectangles(const vector<Rectangle>& rectangles) noexcept;