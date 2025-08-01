#include "pch.h"
#include "GeometryUtil.h"

Rectangle CombineRectangles(const vector<Rectangle>& rectangles) noexcept
{
	if (rectangles.empty())
		return {};

	Rectangle result = rectangles[0];
	for (const Rectangle& rect : rectangles)
		result = Rectangle::Union(result, rect);

	return result;
}