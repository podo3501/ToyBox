#include "pch.h"
#include "Utility.h"
#include "../Toy/Utility.h"

ImVec2 GetWindowStartPosition(const ImGuiWindow* window) noexcept
{
	return window->Pos + ImVec2{ 0, GetFrameHeight() };
}

ImVec2 GetMousePosition(const ImGuiWindow* window) noexcept
{
	const ImVec2& mousePos = ImGui::GetMousePos();
	return mousePos - GetWindowStartPosition(window);
}