#include "pch.h"
#include "TextureResBinderWindow.h"
#include "EditFontTexture.h"
#include "EditSourceTexture.h"
#include "../Utility.h"
#include "../Toy/Utils/StringUtil.h"
#include "../Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/Command/TexResCommandList/TexResCommandList.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIComponent/UIUtility.h"

TextureResBinderWindow::~TextureResBinderWindow()
{
    m_renderer->RemoveImguiComponent(this);
}

TextureResBinderWindow::TextureResBinderWindow(IRenderer* renderer) :
    InnerWindow{ "empty" },
    m_renderer{ renderer },
    m_sourceTexture{ nullptr },
    m_editFontTexture{ make_unique<EditFontTexture>() },
    m_editSourceTexture{ make_unique<EditSourceTexture>(renderer, this) }
{
    m_renderer->AddImguiComponent(this);
}

void TextureResBinderWindow::SetTexture(PatchTextureStd1* pTex1) noexcept
{ 
    m_sourceTexture = pTex1;
    (pTex1) ? SetName(WStringToString(pTex1->GetFilename())) : SetName("empty");
}

bool TextureResBinderWindow::Create(const wstring& filename)
{
    m_resBinder = CreateSourceBinder(filename);
    ReturnIfFalse(m_resBinder);
    m_cmdList = make_unique<TexResCommandList>(m_resBinder.get());

    m_editFontTexture->SetCommandList(m_cmdList.get());
    m_editSourceTexture->SetCommandList(m_cmdList.get());
    m_isOpen = true;
    return true;
}

static bool CheckUndo(TexResCommandList* cmdList)
{
    if (!IsInputAction(Keyboard::LeftControl, Keyboard::Z)) return false;
    return cmdList->Undo();
}

static bool CheckRedo(TexResCommandList* cmdList)
{
    if (!IsInputAction(Keyboard::LeftControl, Keyboard::Y)) return false;
    return cmdList->Redo();
}

bool TextureResBinderWindow::CheckUndoRedo()
{
    auto result = CheckUndo(m_cmdList.get()) || CheckRedo(m_cmdList.get());
    ReturnIfFalse(result);

    m_editSourceTexture->CheckTextureByUndoRedo();
    return true;
}

void TextureResBinderWindow::Update()
{
    if (!m_window) return;
    SetMouseStartOffset(m_window);

    CheckUndoRedo();
    m_editFontTexture->Update();
    m_editSourceTexture->Update();
}

ImVec2 TextureResBinderWindow::GetWindowSize() const noexcept
{
    if (!m_sourceTexture) return ImVec2{ 512, 512 };
    return XMUINT2ToImVec2(m_sourceTexture->GetSize());
}

void TextureResBinderWindow::Render(ImGuiIO* io)
{
    if (!m_isOpen) return;

    const ImVec2& size = GetWindowSize();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    ImGui::Begin(GetName().c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    if (ImGui::IsWindowAppearing())
        m_window = GetImGuiWindow();

    if (m_sourceTexture)
    {
        if (auto offset = m_sourceTexture->GetGraphicMemoryOffset(); offset != 0)
            ImGui::Image(offset, size);
    }

    IgnoreMouseClick(m_window);
    RenderResourceWindow();
    ImGui::End();
}

void TextureResBinderWindow::RenderResourceWindow()
{
    string wndName = string("Texture Resource Window");
    ImGui::Begin(wndName.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    m_editFontTexture->Render();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    m_editSourceTexture->Render();

    ImGui::End();
}

bool TextureResBinderWindow::SaveScene(const wstring& filename)
{
    return m_resBinder->Save(filename);
}

wstring TextureResBinderWindow::GetSaveFilename() const noexcept
{
    return m_resBinder->GetJsonFilename();
}