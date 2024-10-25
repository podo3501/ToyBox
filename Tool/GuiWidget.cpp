#include "pch.h"
#include "GuiWidget.h"
#include "../Toy/Utility.h"

GuiWidget::GuiWidget() 
{};
GuiWidget::~GuiWidget() = default;

bool GuiWidget::Create(IRenderer* renderer, const XMUINT2& size, IRenderItem* renderItem)
{
    ReturnIfFalse(renderer->CreateRenderTexture(size, renderItem, m_textureID));
    m_size = { static_cast<float>(size.x), static_cast<float>(size.y) };

    return true;
}

void GuiWidget::Render(ImGuiIO* io)
{
    //ImGui::Begin("Client Window");
    ImGui::Begin("Horizontal Scrolling Example", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    float gap = 16.f;
    float frameHeight = ImGui::GetFrameHeight();
    ImVec2 size = { m_size.x + gap, m_size.y + gap + frameHeight };
    ImGui::SetWindowSize(size);

    // ImGui에 텍스춰에 찍어논 화면을 연결
    ImGui::Image(m_textureID, m_size);

    ImGui::End();
}