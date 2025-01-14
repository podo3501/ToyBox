#include "pch.h"
#include "EditWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/UIComponentHelper.h"
#include "../Toy/UserInterface/Component/Panel.h"
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

EditWindow::EditWindow(UIComponent* component) noexcept:
	m_component{ component },
    m_dragState{ OnDrag::Normal }
{}

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
        m_component->ChangeSize(modifySize);
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

void EditWindow::RenderCommon(bool& modify)
{
    // Component 이름 수정
    EditText("Name", m_component->Name);

    XMINT2 relativePosition{};
    bool isExist = m_component->GetRelativePosition(relativePosition);
    if(isExist)
    {
        modify |= EditInteger("X", relativePosition.x);
        modify |= EditInteger("Y", relativePosition.y);

        if (modify)
            m_component->SetRelativePosition(relativePosition);
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

bool EditWindow::EditSize(const XMUINT2& size)
{
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= EditInteger("Width", newSize.x);
    changed |= EditInteger("Height", newSize.y);

    if (changed)
        m_component->ChangeSize(newSize);

    return changed;
}

//////////////////////////////////////////////////////////

EditPanel::~EditPanel() = default;
EditPanel::EditPanel(Panel* panel) noexcept :
    EditWindow{ panel }
{
}