#pragma once

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
	return XMINT2((b.x != 0) ? a.x / b.x : 0, (b.y != 0) ? a.y / b.y : 0);
}

inline XMINT2 Vector2ToXMINT2(const Vector2& rhs) noexcept {
	return { static_cast<int32_t>(rhs.x), static_cast<int32_t>(rhs.y) };
}
inline Vector2 XMINT2ToVector2(const XMINT2& rhs) noexcept {
	return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
}

inline XMINT2 XMUINT2ToXMINT2(const XMUINT2& uint2) noexcept { return XMINT2{ static_cast<int32_t>(uint2.x), static_cast<int32_t>(uint2.y) }; };
// XMUINT2�� ���� ��Ģ���� �����ε�
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
