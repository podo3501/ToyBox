#include "pch.h"
#include "Utility.h"
#include "MainWindow/FloatingComponent.h"
#include "../Toy/UserInterface/UIComponentEx.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/Utility.h"

using namespace UIComponentEx;

ImGuiMouseCursor_ Tool::MouseCursor::m_mouseCursor = ImGuiMouseCursor_Arrow;

ImVec2 GetWindowStartPosition(const ImGuiWindow* window) noexcept
{
	return window->Pos + ImVec2{ 0, GetFrameHeight() };
}

ImVec2 GetMousePosition(const ImGuiWindow* window) noexcept
{
	const ImVec2& mousePos = ImGui::GetMousePos();
	return mousePos - GetWindowStartPosition(window);
}

bool IsWindowFocus(const ImGuiWindow* window) noexcept
{
	if (window == nullptr || window->Active == false)
		return false;

	ImGuiWindow* focusedWindow = GImGui->NavWindow;
	if (window != focusedWindow)
		return false;

	return true;
}

static tuple<ImVec2, ImVec2> RectangleToImVec2(const Rectangle& rect)
{
	return { {static_cast<float>(rect.x), static_cast<float>(rect.y) },
		{static_cast<float>(rect.width), static_cast<float>(rect.height) } };
}

void DrawRectangle(const Rectangle& rect, const ImGuiWindow* window)
{
	if (window == nullptr) return;
	if (rect == Rectangle{}) return;

	auto [topLeft, bottomRight] = RectangleToImVec2(rect);
	const ImVec2& windowOffset = GetWindowStartPosition(window);
	topLeft = topLeft + windowOffset;
	bottomRight = topLeft + bottomRight;

	constexpr ImU32 outlineColor = IM_COL32(255, 255, 255, 255);
	constexpr ImU32 fillColor = IM_COL32(255, 255, 255, 100);
	constexpr float thickness = 1.f;

	ImDrawList* drawList = window->DrawList;
	if (!drawList) return;

	drawList->AddRect(topLeft, bottomRight, outlineColor, 0.f, 0, thickness);
	drawList->AddRectFilled(topLeft, bottomRight, fillColor, 0.0f);
}

void DrawRectangle(const vector<Rectangle>& rects, const ImGuiWindow* window)
{
	if (window == nullptr) return;
	if (rects.empty()) return;

	ranges::for_each(rects, [window](const auto& rect) {
		DrawRectangle(rect, window);
		});
}

bool AddComponentFromScreenPos(CommandList* cmdList, UIComponent* addable, FloatingComponent* floater, const XMINT2& pos) noexcept
{
	if (!addable->IsAttachable()) return false;

	const XMINT2& comPos = GetPosition(addable);
	XMINT2 relativePos = pos - comPos;
	unique_ptr<UIComponent> failed = cmdList->AttachComponent(addable, floater->GetComponent(), relativePos);
	if (failed) //제대로 붙지 않았다면 인자로 보낸 Component가 리턴값으로 돌아온다.
	{
		floater->SetComponent(move(failed));
		return false;
	}
	
	floater->Clear(); //마지막으로 클리어 해 줘서 완전히 떼 낸다.
	return true;
}