#include "pch.h"
#include "GuiAppWindow.h"
#include "GuiWidget.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/Utility.h"

GuiAppWindow::~GuiAppWindow() = default;
GuiAppWindow::GuiAppWindow(IRenderer* renderer) :
    m_renderer{ renderer },
    m_renderItem{ nullptr }
{
    m_renderer->AddImguiItem(this);
}

bool GuiAppWindow::Create(IRenderItem* renderItem, const XMUINT2& size)
{
    Dialog* curDialog = static_cast<Dialog*>(renderItem);
    //const Rectangle& area = curDialog->GetArea();
    //XMUINT2 size{ static_cast<uint32_t>(area.width - area.x), static_cast<uint32_t>(area.height - area.y) };
    ReturnIfFalse(m_renderer->CreateRenderTexture(size, renderItem, m_textureID));
    m_renderItem = renderItem;
    m_size = size;

    m_guiWidget = make_unique<GuiWidget>(m_renderer);
    ReturnIfFalse(m_guiWidget->Create(renderItem));

    return true;
}

void GuiAppWindow::Update()
{
    m_guiWidget->Update();
}

void GuiAppWindow::Render(ImGuiIO* io)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("App Window", &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    static bool bPicking{ false };
    ImVec2 curWindowPos = ImGui::GetWindowPos();
    ImVec2 startPos{ curWindowPos.x + 0.f, curWindowPos.y + 0.f + ImGui::GetFrameHeight() };
    if (/*io->MouseDown[0] && */io->MouseReleased[0])
    {
        Vector2 mousePos{ io->MousePos.x, io->MousePos.y };

        Vector2 realMousePos{};
        realMousePos.x = mousePos.x - startPos.x;
        realMousePos.y = mousePos.y - startPos.y;

        //클릭했을때 픽킹이 되었는지 확인
        if (m_renderItem->IsPicking(realMousePos))
            m_guiWidget->SetVisible(true);
    }

    ImGui::Image(m_textureID, { static_cast<float>(m_size.x), static_cast<float>(m_size.y) });
    //ImGui::Image(m_textureID, { static_cast<float>(800), static_cast<float>(600) });
    ImGui::End();
    return;
}