#include "pch.h"
#include "MainTextureWindow.h"
#include "MainSourceExtractor.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"

MainTextureWindow::~MainTextureWindow() = default;
MainTextureWindow::MainTextureWindow(IRenderer* renderer, unique_ptr<MainSourceExtractor> sourceExtractor) :
    m_renderer{ renderer },
    m_sourceExtractor{ move(sourceExtractor) },
    m_sourceTexture{ make_unique<ImageGrid1>() }
{}

bool MainTextureWindow::Create(const wstring& filename)
{
    m_sourceTexture->SetFilenameToLoadInfo(filename);
    ReturnIfFalse(m_renderer->LoadComponent(m_sourceTexture.get()));

    if (const auto& areaList = m_sourceTexture->GetTextureAreaList(); areaList)
        m_sourceExtractor->SetTextureAreaList(*areaList);

    m_name = WStringToString(filename);
    m_isOpen = true;

    return true;
}

void MainTextureWindow::Update()
{
    if (!m_window) return;

    SetMouseStartOffset(m_window);
    m_sourceExtractor->Update();
}

void MainTextureWindow::Render()
{
    if (!m_isOpen) return;

    ImVec2 size = XMUINT2ToImVec2(m_sourceTexture->GetSize());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    //ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
    ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.
    if (ImGui::IsWindowAppearing())
    {
        m_window = ImGui::FindWindowByName(m_name.c_str());
        m_sourceExtractor->SetWindow(m_window);
    }

    ImGui::Image(m_sourceTexture->GetGraphicMemoryOffset(), size);

    m_sourceExtractor->Render();

    ImGui::End();
}