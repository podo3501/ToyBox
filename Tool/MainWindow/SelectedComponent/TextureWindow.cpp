#include "pch.h"
#include "TextureWindow.h"
#include "SourceExtractor.h"
#include "../../Utility.h"
#include "../Include/IRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"

TextureWindow::~TextureWindow() = default;
TextureWindow::TextureWindow(IRenderer* renderer, unique_ptr<SourceExtractor> sourceExtractor) :
    m_renderer{ renderer },
    m_renderTex{ make_unique<RenderTexture>() },
    m_sourceExtractor{ move(sourceExtractor) },
    m_sourceTexture{ make_unique<ImageGrid1>() },
    m_size{}
{}

bool TextureWindow::Create(const wstring& filename)
{
    m_sourceTexture->SetFilenameToLoadInfo(filename);
    ReturnIfFalse(m_renderer->LoadComponent(m_sourceTexture.get()));

    if (const auto& areaList = m_sourceTexture->GetTextureAreaList(); areaList)
        m_sourceExtractor->SetTextureAreaList(*areaList);

    XMUINT2 size = m_sourceTexture->GetSize();
    ReturnIfFalse(m_renderTex->CreateTexture(m_renderer, size, m_sourceTexture.get())); //���� �׸��� �ؽ��翡 �������� �� �ؽ��縦 Render���� �׸���.

    m_size = XMUINT2ToImVec2(size);
    m_name = WStringToString(filename);
    m_isOpen = true;

    return true;
}

void TextureWindow::Update()
{
    if (!m_window) return;
    SetMouseStartOffset(m_window);

    m_sourceExtractor->Update();
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
    if (ImGui::IsWindowAppearing())
    {
        m_window = ImGui::FindWindowByName(m_name.c_str());
        m_sourceExtractor->SetWindow(m_window);
    }

    ImGui::Image(m_renderTex->GetGraphicMemoryOffset(), m_size);

    m_sourceExtractor->Render();

    ImGui::End();
}