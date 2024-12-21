#include "pch.h"
#include "GuiWidget.h"
#include "../Toy/Utility.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UIComponent.h"

GuiWidget::~GuiWidget()
{}

GuiWidget::GuiWidget(IRenderer* renderer) :
    m_renderer{ renderer },
    m_component{ nullptr }
{
    m_renderer->AddImguiComponent(this);
}

bool GuiWidget::Create(unique_ptr<UIComponent> comp)
{
    const string& name = comp->GetName();
    //const Rectangle& area = comp->GetSizeR();

    /*m_widgetScene->AddComponent({ 0.f, 0.f }, move(comp));
    m_component = m_widgetScene->GetComponent(name);
    
    XMUINT2 size{ static_cast<uint32_t>(area.width - area.x), static_cast<uint32_t>(area.height - area.y) };
    ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_widgetScene.get(), m_textureID));*/

    return true;    
}

void GuiWidget::Update()
{
    //m_widgetScene->Update(nullptr);
    //m_component->Update({ 0.f, 0.f }, nullptr);
}

void GuiWidget::Render(ImGuiIO* io)
{
    if (!m_visible) return;

    ////ImGui::Begin("Client Window");
    ////������ �����Ҷ� �� ������ �ִ´�.
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 30.0f));
    ////�ι�° ���ڰ� bool ���� ���� �ݱ�(x) ��ư�� �����.
    ////ImGuiWindowFlags_AlwaysAutoResize ũ�Ⱑ �׻� �����ϴ�.
    //ImGui::Begin("Horizontal Scrolling Example", &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
    //ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.
    //
    //static bool bPicking{ false };
    //ImVec2 curWindowPos = ImGui::GetWindowPos();
    //ImVec2 startPos{ curWindowPos.x + 30.f, curWindowPos.y + 30 + ImGui::GetFrameHeight() };
    //if (/*io->MouseDown[0] && */io->MouseReleased[0])
    //{
    //    Vector2 mousePos{ io->MousePos.x, io->MousePos.y };

    //    Vector2 realMousePos{};
    //    realMousePos.x = mousePos.x - startPos.x;
    //    realMousePos.y = mousePos.y - startPos.y;

    //    //Ŭ�������� ��ŷ�� �Ǿ����� Ȯ��
    //    //if (m_component->IsPicking(XMINT2{ static_cast<int32_t>(realMousePos.x), static_cast<int32_t>(realMousePos.y) }))
    //       // bPicking = !bPicking;
    //}

    ////BGImage* bgImage = static_cast<BGImage*>(m_component);
    //static Rectangle newArea = { 0, 0, m_component->GetSize().x, m_component->GetSize().y };

    //// ImGui�� �ؽ��翡 ���� ȭ���� ����
    //ImGui::Image(m_textureID, { static_cast<float>(newArea.width), static_cast<float>(newArea.height) });
    //
    //if(bPicking)
    //{
    //    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //    const Rectangle& rect = { 0, 0, m_component->GetSize().x, m_component->GetSize().y };

    //    static ImVec4 colf = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
    //    const ImU32 col = ImColor(colf);
    //    //���߿� �׸��� ���� ���� �ö�´�.
    //    draw_list->AddRectFilled(
    //        { static_cast<float>(rect.x) + startPos.x, static_cast<float>(rect.y) + startPos.y },
    //        { static_cast<float>(rect.x) + static_cast<float>(rect.width) + startPos.x, 
    //        static_cast<float>(rect.y) + static_cast<float>(rect.height) + startPos.y }, col);

    //    ImGui::Begin("Dialog Property", &bPicking);
    //    const Rectangle& dialogArea = { 0, 0, m_component->GetSizeR().x, m_component->GetSizeR().y };

    //    static int width = dialogArea.width;
    //    ImGui::InputInt("width", &width);
    //    static int height = dialogArea.height;
    //    ImGui::InputInt("height", &height);

    //    newArea.width = width;
    //    newArea.height = height;
    //    if (dialogArea != newArea)
    //    {
    //        //m_component->ChangeArea(newArea);
    //        //m_renderer->ModifyRenderTexture(m_textureID, { static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
    //    }
    //    ImGui::End();
    //}

    //ImGui::End();
}