#include "pch.h"
#include "TextureWindow.h"
#include "../Include/IRenderer.h"
#include "ExtractTexture.h"
#include "../Toy/Utility.h"
#include "../../Utility.h"
#include "SourceExtractor.h"

TextureWindow::~TextureWindow()
{
    m_renderer->RemoveRenderTexture(m_textureID);
    m_renderer->RemoveRenderComponent(m_extractTexture.get());
}

TextureWindow::TextureWindow(IRenderer* renderer, unique_ptr<SourceExtractor> sourceExtractor) :
    m_renderer{ renderer },
    m_sourceExtractor{ move(sourceExtractor) },
    m_extractTexture{ make_unique<ExtractTexture>() },
    m_size{}
{}

bool TextureWindow::Create(const wstring& filename)
{
    m_extractTexture->SetImage(filename);
    ReturnIfFalse(m_renderer->LoadComponent(m_extractTexture.get()));

    XMUINT2 size = m_extractTexture->GetSize();
    ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_extractTexture.get(), m_textureID));

    m_size = XMUINT2ToImVec2(size);
    m_name = WStringToString(filename);
    m_isOpen = true;

    return true;
}

void TextureWindow::Render()
{
    if (!m_isOpen)
        return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ m_size.x, m_size.y + GetFrameHeight() });
    //ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
    ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.

    ImGui::Image(m_textureID, m_size);

    ImGuiWindow* window = ImGui::FindWindowByName(m_name.c_str());
    m_sourceExtractor->Render(window);

    ImGui::End();
}