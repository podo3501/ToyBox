#pragma once

inline float GetFrameHeight()
{
	return ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
}

inline XMUINT2 ImVec2ToXMUINT2(const ImVec2& vec)
{
	return { static_cast<uint32_t>(vec.x), static_cast<uint32_t>(vec.y) };
}

inline ImVec2 XMUINT2ToImVec2(const XMUINT2& uint2)
{
	return { static_cast<float>(uint2.x), static_cast<float>(uint2.y) };
}

inline bool operator==(const ImVec2& lhs, const ImVec2& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const ImVec2& lhs, const ImVec2& rhs)
{
	return !(lhs == rhs);
}
