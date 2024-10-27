#include "pch.h"
#include "GuiWidget.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Dialog.h"

GuiWidget::~GuiWidget() = default;
GuiWidget::GuiWidget(IRenderer* renderer) :
    m_renderer{ renderer },
    m_renderItem{ nullptr }
{
    m_renderer->AddImguiItem(this);
};

bool GuiWidget::Create(IRenderItem* renderItem)
{
    Dialog* curDialog = static_cast<Dialog*>(renderItem);
    const Rectangle& area = curDialog->GetArea();
    XMUINT2 size{ static_cast<uint32_t>(area.width - area.x), static_cast<uint32_t>(area.height - area.y) };
    ReturnIfFalse(m_renderer->CreateRenderTexture(size, renderItem, m_textureID));
    m_renderItem = renderItem;

    return true;    
}

void GuiWidget::Update()
{
}

bool bVisible = true;
void GuiWidget::Render(ImGuiIO* io)
{
    if (!bVisible) return;

    //ImGui::Begin("Client Window");
    //������ �����Ҷ� �� ������ �ִ´�.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 30.0f));
    //�ι�° ���ڰ� bool ���� ���� �ݱ�(x) ��ư�� �����.
    //ImGuiWindowFlags_AlwaysAutoResize ũ�Ⱑ �׻� �����ϴ�.
    ImGui::Begin("Horizontal Scrolling Example", &bVisible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.
    
    static bool bPicking{ false };
    ImVec2 curWindowPos = ImGui::GetWindowPos();
    ImVec2 startPos{ curWindowPos.x + 30.f, curWindowPos.y + 30 + ImGui::GetFrameHeight() };
    if (/*io->MouseDown[0] && */io->MouseReleased[0])
    {
        Vector2 mousePos{ io->MousePos.x, io->MousePos.y };

        Vector2 realMousePos{};
        realMousePos.x = mousePos.x - startPos.x;
        realMousePos.y = mousePos.y - startPos.y;

        //Ŭ�������� ��ŷ�� �Ǿ����� Ȯ��
        if (m_renderItem->IsPicking(realMousePos))
            bPicking = !bPicking;
    }

    Dialog* curDialog = static_cast<Dialog*>(m_renderItem);
    static Rectangle newArea = curDialog->GetArea();

    // ImGui�� �ؽ��翡 ���� ȭ���� ����
    ImGui::Image(m_textureID, { static_cast<float>(newArea.width), static_cast<float>(newArea.height) });
    
    if(bPicking)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const Rectangle& rect = m_renderItem->GetArea();

        static ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
        const ImU32 col = ImColor(colf);
        //���߿� �׸��� ���� ���� �ö�´�.
        draw_list->AddRectFilled(
            { static_cast<float>(rect.x) + startPos.x, static_cast<float>(rect.y) + startPos.y },
            { static_cast<float>(rect.x) + static_cast<float>(rect.width) + startPos.x, 
            static_cast<float>(rect.y) + static_cast<float>(rect.height) + startPos.y }, col);

        ImGui::Begin("Dialog Property", &bPicking);
        const Rectangle& dialogArea = curDialog->GetArea();

        static int width = dialogArea.width;
        ImGui::InputInt("width", &width);
        static int height = dialogArea.height;
        ImGui::InputInt("height", &height);

        newArea.width = width;
        newArea.height = height;
        if (dialogArea != newArea)
        {
            curDialog->ChangeArea(newArea);
            m_renderer->ModifyRenderTexture(m_textureID, { static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
        }
        ImGui::End();
    }

    ImGui::End();
}