#include "pch.h"
#include "EditWindow.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"
#include "Toy/UserInterface/CommandHistory/UserInterface/UICommandHistory.h"
#include "Shared/System/Input.h"
#include "Shared/Utils/GeometryExt.h"
#include "EditUtility/EditUtility.h"
#include "HelperClass.h"
#include "Utility.h"


enum class OnDrag
{
    Normal,
    Bottom,
    Right,
    Corner,
};

EditWindow::~EditWindow() = default;
EditWindow::EditWindow(UIComponent* component, UICommandHistory* cmdHistory) noexcept:
	m_component{ component },
    m_cmdHistory{ cmdHistory },
    m_renameNotifier{ make_unique<RenameNotifier>() },
    m_dragState{ OnDrag::Normal }
{}

void EditWindow::Setup()
{
    SetupComponent();
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

static OnDrag IsMouseOverResizeZone(const XMINT2& pos, const UIComponent* component) noexcept
{
    auto zones = GenerateResizeZone(component->GetArea(), 8);
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

void EditWindow::UpdateDragState(OnDrag dragState, XMINT2& outStartPos) noexcept
{
    const auto& mouseState = Input::GetMouse().GetLastState();

    if (IsInputAction(MouseButton::Left, KeyState::Pressed) && dragState != OnDrag::Normal)
    {
        m_dragState = dragState;
        outStartPos = { mouseState.x, mouseState.y };
    }

    if (IsInputAction(MouseButton::Left, KeyState::Released))
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
        m_cmdHistory->SetSize(m_component, modifySize);
}

void EditWindow::ResizeComponentOnClick() noexcept
{
    if (!m_component) return;

    const XMINT2& mousePos = Input::GetMouse().GetPosition();
    OnDrag dragState = IsMouseOverResizeZone(mousePos, m_component);
    Tool::MouseCursor::SetType(GetCursorImage(dragState));

    static XMINT2 startPos{};
    UpdateDragState(dragState, startPos);

    if (IsUpdateSizeOnDrag())
    {
        const auto& mouseTracker = Input::GetMouse();
        const auto& mouseState = mouseTracker.GetLastState();
        ResizeComponent(startPos, mouseState);
        startPos = { mouseState.x, mouseState.y };
    }
}

void EditWindow::Update(bool mainWndFocus)
{
    if (mainWndFocus)
        ResizeComponentOnClick();

    UpdateComponent();
}

void EditWindow::RenderCommon()
{
    m_renameNotifier->EditName("Name", m_component->GetName(), [this](const std::string& newName) {
        return m_cmdHistory->Rename(m_component, newName); });

    XMINT2 relativePosition = m_component->GetRelativePosition();   
    if(EditPosition(relativePosition))
        m_cmdHistory->SetRelativePosition(m_component, relativePosition);

    XMUINT2 size{ m_component->GetSize() };
    if (EditSize(size))
        m_cmdHistory->SetSize(m_component, size);

    string region = m_component->GetRegion();
    if (EditText("Region", region))
        m_cmdHistory->RenameRegion(m_component, region);

    ImGui::Separator();
    ImGui::Spacing();
}

void EditWindow::Render(const ImGuiWindow* uiWindow)
{
    if (!m_visible) return;
    
    string wndName = string("Component Window - ") + string(uiWindow->Name);
    ImGui::Begin(wndName.c_str(), &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);
    
    RenderCommon();
    RenderComponent();

    ImGui::End();
}

//////////////////////////////////////////////////////////

EditPanel::~EditPanel() = default;
EditPanel::EditPanel(Panel* panel, UICommandHistory* cmdHistory) noexcept :
    EditWindow{ panel, cmdHistory }
{
}