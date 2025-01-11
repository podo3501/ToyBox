#include "pch.h"
#include "EditWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/Panel.h"
#include "../EditUtility.h"
#include "../Toy/InputManager.h"
#include "../Toy/HelperClass.h"
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
    m_drag{ OnDrag::Normal }
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

static OnDrag IsMouseOverResizeZone(InputManager* inputManager, const UIComponent* component) noexcept
{
    if (!inputManager || !component) return OnDrag::Normal;

    const auto& pos = inputManager->GetMouse()->GetOffsetPosition();
    const Rectangle& rect = component->GetRectangle();
    auto zones = GenerateResizeZone(rect, 8);

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
    return (m_drag != OnDrag::Normal);
}

void EditWindow::UpdateMouseCursor(InputManager* inputManager) noexcept
{
    m_drag = IsMouseOverResizeZone(inputManager, m_component);

    Tool::MouseCursor::SetType(GetCursorImage(m_drag));
}

void EditWindow::ResizeComponentOnClick(InputManager* inputManager) noexcept
{
    auto mouseTracker = inputManager->GetMouse();
    static bool m_dragOn = false;
    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED && m_drag != OnDrag::Normal)
        m_dragOn = true;
    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::RELEASED && m_drag != OnDrag::Normal)
        m_dragOn = false;
    
    if (!m_dragOn) return;

    auto mouseState = mouseTracker->GetLastState();
    static POINT lastMousePos = { mouseState.x, mouseState.y };

    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::HELD && m_dragOn) 
    {
        int deltaX = mouseState.x - lastMousePos.x;
        int deltaY = mouseState.y - lastMousePos.y;

        XMUINT2 size = m_component->GetSize();
        size.x += deltaX;
        size.y += deltaY;
        m_component->SetSize(size);

        // 이전 위치 업데이트
        lastMousePos.x = mouseState.x;
        lastMousePos.y = mouseState.y;
    }

    //XMINT2 curPosition = mouseTracker->GetOffsetPosition();
    //if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
    //{
    //    pressedPosition = curPosition;
    //    return;
    //}

    //if (mouseTracker->leftButton == Mouse::ButtonStateTracker::HELD)
    //{
    //    XMINT2 movePosition = curPosition - pressedPosition;

    //}
    XMUINT2 size = m_component->GetSize();
}

void EditWindow::Update(InputManager* inputManager, bool mainWndFocus)
{
    if (mainWndFocus)
    {
        UpdateMouseCursor(inputManager);
        ResizeComponentOnClick(inputManager);
    }

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