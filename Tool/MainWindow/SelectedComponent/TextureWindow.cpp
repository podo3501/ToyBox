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
    m_sourceExtractor{ move(sourceExtractor) },
    m_sourceTexture{ make_unique<ImageGrid1>() }
{}

bool TextureWindow::Create(const wstring& filename)
{
    //?!? LoadTextureBinder를 써서 로딩한후 ImageGrid1에 SetSourceInfo 함수로 연결한다.
    //m_sourceTexture->SetFilenameToLoadInfo(filename);
    //ReturnIfFalse(m_renderer->LoadComponent(m_sourceTexture.get()));

    if (const auto& areaList = m_sourceTexture->GetTextureAreaList(); areaList)
        m_sourceExtractor->SetTextureAreaList(*areaList);

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
    if (!m_isOpen) return;

    ImVec2 size = XMUINT2ToImVec2(m_sourceTexture->GetSize());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    //ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
    ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.
    if (ImGui::IsWindowAppearing())
    {
        m_window = ImGui::FindWindowByName(m_name.c_str());
        m_sourceExtractor->SetWindow(m_window);
    }

    ImGui::Image(m_sourceTexture->GetGraphicMemoryOffset(), size);

    m_sourceExtractor->Render();

    ImGui::End();
}