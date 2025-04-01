#pragma once
#include "../Toy/UserInterface/UIComponent/UIType.h"

inline float GetFrameHeight()
{
	return ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
}

ImVec2 GetWindowStartPosition(const ImGuiWindow* window) noexcept;
ImVec2 GetWindowIGMousePos(const ImGuiWindow* window) noexcept;
void SetMouseStartOffset(const ImGuiWindow* window) noexcept;
bool IsWindowFocus(const ImGuiWindow* window) noexcept;
void DrawRectangle(const ImGuiWindow* window, const Rectangle& rect, 
	optional<ImU32> outlineColor = nullopt, optional<ImU32> fillColor = nullopt);
void DrawRectangles(const ImGuiWindow* window, const vector<Rectangle>& rects,
	optional<ImU32> outlineColor = nullopt, optional<ImU32> fillColor = nullopt);

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

inline Vector2 GetNormalPosition(const XMINT2& position, const XMUINT2& size)
{
	return {
		static_cast<float>(position.x) / static_cast<float>(size.x),
		static_cast<float>(position.y) / static_cast<float>(size.y)
	};
}

namespace Tool
{
	//���콺 ���¸� �ٲٴ� ���� �ַ� Update �Լ����� �ϴµ� Render���� ������ �ؾ� �ؼ� static ���� ����.
	class MouseCursor
	{
	public:
		inline static void SetType(ImGuiMouseCursor_ cursorType) noexcept { m_mouseCursor = cursorType; }
		inline static void Render() noexcept { ImGui::SetMouseCursor(m_mouseCursor); }

	private:
		static ImGuiMouseCursor_ m_mouseCursor;
	};
}
void IgnoreMouseClick(ImGuiWindow* window);

class UIComponent;
class UICommandList;
class FloatingComponent;

inline ImColor ToColor(const XMVECTORF32& color, float alpha = 1.f) noexcept {
	return {
		static_cast<int>(color.f[0] * 255.0f),
		static_cast<int>(color.f[1] * 255.0f),
		static_cast<int>(color.f[2] * 255.0f),
		static_cast<int>(alpha * 255.0f) };
}

bool AddComponentFromScreenPos(UICommandList* cmdList, UIComponent* addable, FloatingComponent* floater, const XMINT2& pos) noexcept;
wstring ReplaceFileExtension(const wstring& filename, const wstring& newExtension);