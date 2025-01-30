#pragma once

inline float GetFrameHeight()
{
	return ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
}

ImVec2 GetWindowStartPosition(const ImGuiWindow* window) noexcept;
ImVec2 GetWindowIGMousePos(const ImGuiWindow* window) noexcept;
XMINT2 GetWindowMousePos(const ImGuiWindow* window) noexcept;
bool IsWindowFocus(const ImGuiWindow* window) noexcept;
void DrawRectangle(const Rectangle& rect, const ImGuiWindow* window);
void DrawRectangle(const vector<Rectangle>& rects, const ImGuiWindow* window);

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
	//마우스 상태를 바꾸는 곳은 주로 Update 함수에서 하는데 Render에서 지정을 해야 해서 static 으로 만듦.
	class MouseCursor
	{
	public:
		inline static void SetType(ImGuiMouseCursor_ cursorType) noexcept { m_mouseCursor = cursorType; }
		inline static void Render() noexcept { ImGui::SetMouseCursor(m_mouseCursor); }

	private:
		static ImGuiMouseCursor_ m_mouseCursor;
	};
}

class UIComponent;
class CommandList;
class FloatingComponent;

inline ImVec4 ToColor(const DirectX::XMVECTORF32& color) noexcept
{
	return ImVec4(color.f[0], color.f[1], color.f[2], color.f[3]);
}

bool AddComponentFromScreenPos(CommandList* cmdList, UIComponent* addable, FloatingComponent* floater, const XMINT2& pos) noexcept;