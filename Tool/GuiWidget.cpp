#include "pch.h"
#include "GuiWidget.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/BGImage.h"

GuiWidget::~GuiWidget()
{
    int a = 1;
}

GuiWidget::GuiWidget(IRenderer* renderer) :
    m_renderer{ renderer },
    m_renderItem{ nullptr }
{
    m_renderer->AddImguiItem(this);
};

bool GuiWidget::Create(unique_ptr<IRenderItem> renderItem)
{
    //BGImage* bgImage = static_cast<BGImage*>(renderItem);
    const Rectangle& area = renderItem->GetArea();
    XMUINT2 size{ static_cast<uint32_t>(area.width - area.x), static_cast<uint32_t>(area.height - area.y) };
    ReturnIfFalse(m_renderer->CreateRenderTexture(size, renderItem.get(), m_textureID));
    m_renderItem = move(renderItem);

    return true;    
}

void GuiWidget::Update()
{
    m_renderItem->Update({ 0.f, 0.f });
}

void GuiWidget::Render(ImGuiIO* io)
{
    if (!m_visible) return;

    //ImGui::Begin("Client Window");
    //������ �����Ҷ� �� ������ �ִ´�.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 30.0f));
    //�ι�° ���ڰ� bool ���� ���� �ݱ�(x) ��ư�� �����.
    //ImGuiWindowFlags_AlwaysAutoResize ũ�Ⱑ �׻� �����ϴ�.
    ImGui::Begin("Horizontal Scrolling Example", &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
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

    //BGImage* bgImage = static_cast<BGImage*>(m_renderItem);
    static Rectangle newArea = m_renderItem->GetArea();

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
        const Rectangle& dialogArea = m_renderItem->GetArea();

        static int width = dialogArea.width;
        ImGui::InputInt("width", &width);
        static int height = dialogArea.height;
        ImGui::InputInt("height", &height);

        newArea.width = width;
        newArea.height = height;
        if (dialogArea != newArea)
        {
            //m_renderItem->ChangeArea(newArea);
            //m_renderer->ModifyRenderTexture(m_textureID, { static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
        }
        ImGui::End();
    }

    ImGui::End();
}