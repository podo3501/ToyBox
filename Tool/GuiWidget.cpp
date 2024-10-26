#include "pch.h"
#include "GuiWidget.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Dialog.h"

GuiWidget::GuiWidget() : 
    m_renderItem{ nullptr }
{};
GuiWidget::~GuiWidget() = default;

XMUINT2 createSize{};
IRenderer* g_renderer{ nullptr };
bool GuiWidget::Create(IRenderer* renderer, const XMUINT2& size, IRenderItem* renderItem)
{
    ReturnIfFalse(renderer->CreateRenderTexture(size, renderItem, m_textureID));
    ReturnIfFalse(renderer->CreateRenderTexture(size, renderItem, m_textureID));
    m_size = { static_cast<float>(size.x), static_cast<float>(size.y) };
    createSize.x = uint32_t(m_size.x);
    createSize.y = uint32_t(m_size.y);
    m_renderItem = renderItem;
    g_renderer = renderer;

    return true;    
}

bool g_bCreate{ false };

void GuiWidget::Update()
{
    if(g_bCreate)
    {
        g_renderer->CreateRenderTexture(createSize, m_renderItem, m_textureID);
        g_bCreate = false;
    }
}

bool bVisible = true;
bool bDialogVisible = true;
void GuiWidget::Render(ImGuiIO* io)
{
    if (not bVisible) return;

    //ImGui::Begin("Client Window");
    //윈도우 생성할때 빈 공간을 넣는다.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 30.0f));
    //두번째 인자가 bool 값이 들어가면 닫기(x) 버튼이 생긴다.
    //ImGuiWindowFlags_AlwaysAutoResize 크기가 항상 일정하다.
    ImGui::Begin("Horizontal Scrolling Example", &bVisible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.
    
    static bool bPicking{ false };
    ImVec2 curWindowPos = ImGui::GetWindowPos();
    ImVec2 startPos{ curWindowPos.x + 30.f, curWindowPos.y + 30 + ImGui::GetFrameHeight() };
    if (/*io->MouseDown[0] && */io->MouseReleased[0])
    {
        Vector2 mousePos{ io->MousePos.x, io->MousePos.y };

        Vector2 realMousePos{};
        realMousePos.x = mousePos.x - startPos.x;
        realMousePos.y = mousePos.y - startPos.y;

        //클릭했을때 픽킹이 되었는지 확인
        if (m_renderItem->IsPicking(realMousePos))
            bPicking = !bPicking;
    }

    if(bPicking)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const Rectangle& rect = m_renderItem->GetArea();

        static ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
        const ImU32 col = ImColor(colf);
        draw_list->AddRectFilled(
            { static_cast<float>(rect.x) + startPos.x, static_cast<float>(rect.y) + startPos.y },
            { static_cast<float>(rect.x) + static_cast<float>(rect.width) + startPos.x, 
            static_cast<float>(rect.y) + static_cast<float>(rect.height) + startPos.y }, col);

        ImGui::Begin("Dialog Property", &bDialogVisible);
        Dialog* curDialog = static_cast<Dialog*>(m_renderItem);
        const Rectangle& dialogArea = curDialog->GetArea();

        static int i0 = dialogArea.x;
        ImGui::InputInt("x", &i0);
        static int i1 = dialogArea.y;
        ImGui::InputInt("y", &i1);
        static int i2 = dialogArea.width;
        ImGui::InputInt("width", &i2);
        static int i3 = dialogArea.height;
        ImGui::InputInt("height", &i3);

        Rectangle newArea{ i0, i1, i2, i3 };
        if (dialogArea != newArea)
        {
            curDialog->ChangeArea(newArea);
            createSize.x = static_cast<uint32_t>(i2 - i0);
            createSize.y = static_cast<uint32_t>(i3 - i1);
            g_bCreate = true;
        }

        ImGui::End();
    }

    // ImGui에 텍스춰에 찍어논 화면을 연결
    
    ImGui::Image(m_textureID, { float(createSize.x), float(createSize.y) });

    ImGui::End();
}