#include "pch.h"
#include "EditWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/UIComponentHelper.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../EditUtility.h"
#include "../Toy/InputManager.h"
#include "../../Utility.h"
#include "../Toy/Utility.h"

enum class OnDrag
{
    Normal,
    Bottom,
    Right,
    Corner,
};

EditWindow::~EditWindow()
{}

EditWindow::EditWindow(UIComponent* component, CommandList* cmdList) noexcept:
	m_component{ component },
    m_cmdList{ cmdList },
    m_dragState{ OnDrag::Normal }
{
    StringToChar(m_component->GetName(), m_nameBuffer);
}

static vector<pair<Rectangle, OnDrag>> GenerateResizeZone(
    const Rectangle& rect, long padding) noexcept
{
    const long safePaddedWidth = max<long>(rect.width - padding, 0);
    const long doublePadding = padding * 2;

    return {
        { { rect.x, rect.y + rect.height, safePaddedWidth, padding }, OnDrag::Bottom },
        { { rect.x + rect.width, rect.y, padding, rect.height }, OnDrag::Right },
        { { rect.x + safePaddedWidth, rect.y + rect.height, doublePadding, padding }, OnDrag::Corner }
    };
}

static OnDrag IsMouseOverResizeZone(const MouseTracker& mouseTracker, const UIComponent* component) noexcept
{
    const auto& pos = mouseTracker.GetOffsetPosition();
    auto zones = GenerateResizeZone(GetRectangle(component), 8);

    for (const auto& zone : zones)
    {
        if (Contains(zone.first, pos))
            return zone.second;
    }

    return OnDrag::Normal;
}

static ImGuiMouseCursor_ GetCursorImage(OnDrag curDrag) noexcept
{
    switch (curDrag)
    {
    case OnDrag::Normal: return ImGuiMouseCursor_Arrow;
    case OnDrag::Bottom: return ImGuiMouseCursor_ResizeNS;
    case OnDrag::Right: return ImGuiMouseCursor_ResizeEW;
    case OnDrag::Corner: return ImGuiMouseCursor_ResizeNWSE;
    }

    return ImGuiMouseCursor_Arrow;
}

bool EditWindow::IsUpdateSizeOnDrag() const noexcept
{
    return (m_dragState != OnDrag::Normal);
}

void EditWindow::UpdateDragState(OnDrag dragState, const MouseTracker& mouseTracker, XMINT2& outStartPos) noexcept
{
    const auto& mouseState = mouseTracker.GetLastState();

    if (IsInputAction(mouseTracker, MouseButton::Left, KeyState::Pressed) && dragState != OnDrag::Normal)
    {
        m_dragState = dragState;
        outStartPos = { mouseState.x, mouseState.y };
    }

    if (IsInputAction(mouseTracker, MouseButton::Left, KeyState::Released))
    {
        m_dragState = OnDrag::Normal;
        outStartPos = {};
    }
}

void EditWindow::ResizeComponent(const XMINT2& startPos, const Mouse::State& mouseState) noexcept
{
    const int deltaX = mouseState.x - startPos.x;
    const int deltaY = mouseState.y - startPos.y;

    const XMUINT2& size = m_component->GetSize();
    XMUINT2 modifySize{ size };

    switch (m_dragState)
    {
    case OnDrag::Bottom: modifySize.y += deltaY; break;
    case OnDrag::Right: modifySize.x += deltaX; break;
    case OnDrag::Corner:
        modifySize.x += deltaX;
        modifySize.y += deltaY;
        break;
    default: break;
    }

    if (size != modifySize)
    {
        m_cmdList->Size(m_component, modifySize);
        m_component->RefreshPosition();
    }
}

void EditWindow::ResizeComponentOnClick(const InputManager& inputManager) noexcept
{
    if (!m_component) return;

    const auto& mouseTracker = inputManager.GetMouse();
    const auto& mouseState = mouseTracker.GetLastState();

    OnDrag dragState = IsMouseOverResizeZone(mouseTracker, m_component);
    Tool::MouseCursor::SetType(GetCursorImage(dragState));

    static XMINT2 startPos{};
    UpdateDragState(dragState, mouseTracker, startPos);

    if (IsUpdateSizeOnDrag())
    {
        ResizeComponent(startPos, mouseState);
        startPos = { mouseState.x, mouseState.y };
    }
}

void EditWindow::Update(const InputManager& inputManager, bool mainWndFocus)
{
    if (mainWndFocus)
        ResizeComponentOnClick(inputManager);

    UpdateComponent(inputManager);
}

static void ShowEditNameResult(chrono::steady_clock::time_point startTime, bool result, bool &visible) noexcept
{
    if (!visible) return;
    
    auto currentTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

    if (elapsedTime >= 2)
    {
        visible = false;
        return;
    }
    
    auto [color, message] = result
        ? std::make_tuple(ToColor(Colors::Yellow), "Name has been successfully changed.")
        : std::make_tuple(ToColor(Colors::Red), "The name is not unique.");

    ImGui::TextColored(color, "%s", message);
}

void EditWindow::EditName(const string& nameLabel) noexcept
{
    static bool resultVisible = false;
    static chrono::steady_clock::time_point startTime{};
    static bool result{ false };

    ImGui::InputText(nameLabel.c_str(), m_nameBuffer, IM_ARRAYSIZE(m_nameBuffer));
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        result = m_cmdList->Rename(m_component, m_nameBuffer);
        startTime = std::chrono::steady_clock::now();
        resultVisible = true;
    }
    StringToChar(m_component->GetName(), m_nameBuffer); //바꾸는데 실패하면 이전 이름으로 돌림. 그리고 Undo 했을때 이름이 바뀌면 업데이트 된다.

    ShowEditNameResult(startTime, result, resultVisible);
}

bool EditWindow::EditSize(const XMUINT2& size)
{
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= EditInteger("Width", newSize.x);
    changed |= EditInteger("Height", newSize.y);

    if (changed)
        m_cmdList->Size(m_component, newSize);

    return changed;
}

void EditWindow::RenderCommon(bool& modify)
{
    EditName("Name");

    auto relativePosition = m_component->GetRelativePosition();
    if(relativePosition.has_value())
    {
        modify |= EditInteger("X", relativePosition->x);
        modify |= EditInteger("Y", relativePosition->y);

        if (modify)
            m_cmdList->RelativePosition(m_component, *relativePosition);
    }

    const auto& layout = m_component->GetLayout();
    modify |= EditSize(layout.GetSize());

    ImGui::Separator();
    ImGui::Spacing();
}

void EditWindow::Render(const ImGuiWindow* mainWindow)
{
    if (!m_visible) return;
    
    string wndName = string("Component Window - ") + string(mainWindow->Name);
    ImGui::Begin(wndName.c_str(), &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);
    
    bool modify{ false };

    RenderCommon(modify);
    RenderComponent(modify);

    if (modify)
        m_component->RefreshPosition();

    ImGui::End();
}

//////////////////////////////////////////////////////////

EditPanel::~EditPanel() = default;
EditPanel::EditPanel(Panel* panel, CommandList* cmdList) noexcept :
    EditWindow{ panel, cmdList }
{
}