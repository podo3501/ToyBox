#include "pch.h"
#include "../Include/IRenderer.h"
#include "EditSourceTexture.h"
#include "TextureSourceWindow.h"
#include "ImageSelector.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureLoadBinder.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Dialog.h"
#include "../Toy/Utility.h"

enum class PendingAction : int
{
    LoadTextureFile,
    DeleteTextureFile,
    SelectTextureFile
};

EditSourceTexture::~EditSourceTexture() = default;
EditSourceTexture::EditSourceTexture(IRenderer* renderer, TextureSourceWindow* textureWindow) :
    m_renderer{ renderer },
    m_textureWindow{ textureWindow },
    m_textureLoader{ make_unique<TextureLoadBinder>() },
    m_sourceBinder{ nullptr },
    m_imageSelector{ make_unique<ImageSelector>(textureWindow) }
{}

void EditSourceTexture::ApplyTexture(ImageGrid1* tex) const noexcept
{
    m_textureWindow->SetTexture(tex);
    m_imageSelector->SetTexture(tex);
}

bool EditSourceTexture::LoadTextureFromFile(const wstring& filename)
{
    m_textureLoader->AddTexture(filename);
    m_renderer->LoadTextureBinder(m_textureLoader.get());
    auto sourceInfo = m_textureLoader->GetSourceInfo(filename);
    ReturnIfFalse(sourceInfo);

    unique_ptr<ImageGrid1> texture = make_unique<ImageGrid1>();
    texture->SetSourceInfo(*sourceInfo, m_renderer->GetTextureController());
    AddTexture(move(texture));

    return true;
}

bool EditSourceTexture::SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept
{
    m_sourceBinder = sourceBinder;
    m_imageSelector->SetSourceBinder(sourceBinder);
    
    const auto& texFiles = m_sourceBinder->GetTextureFiles();
    if (texFiles.empty()) return true;

    for (auto& texFile : texFiles)
        ReturnIfFalse(LoadTextureFromFile(texFile));
    
    SelectDefaultTextureFile();
    return true;
}

bool EditSourceTexture::LoadTextureFile()
{
    wstring filename;
    GetRelativePathFromDialog(filename);
    if (filename.empty() || IsLoadedTexture(filename)) return true;
    ReturnIfFalse(LoadTextureFromFile(filename));

    return true;
}

bool EditSourceTexture::ExecuteAction() noexcept
{
    if (!m_pendingAction.has_value()) return true;

    auto result{ true };
    switch (*m_pendingAction)
    {
    case PendingAction::LoadTextureFile: result = LoadTextureFile(); break;
    case PendingAction::DeleteTextureFile: result = DeleteTextureFile(); break;
    case PendingAction::SelectTextureFile: result = SelectTextureFile(); break;
    default: break;
    }

    m_pendingAction.reset(); // 상태 초기화

    return result;
}

void EditSourceTexture::Update() noexcept
{
    ExecuteAction();
    m_imageSelector->Update();
}

void EditSourceTexture::Render()
{
    RenderTextureList();
    m_imageSelector->Render();
}

bool EditSourceTexture::IsLoadedTexture(const wstring& filename) const noexcept
{
    auto it = ranges::find_if(m_textureFiles, [&filename](auto& tex) { return tex->GetFilename() == filename; });
    return (it != m_textureFiles.end()) ? true : false;
}

void EditSourceTexture::AddTexture(unique_ptr<ImageGrid1> texture) noexcept
{
    m_textureFiles.emplace_back(move(texture));
    m_texIndex = static_cast<int>(m_textureFiles.size() - 1);
    SelectTextureFile();
}

void EditSourceTexture::RemoveTexture(int textureIdx) noexcept
{
    m_textureFiles.erase(m_textureFiles.begin() + m_texIndex);
    m_texIndex = m_textureFiles.size() ? 0 : -1; 
    SelectTextureFile();
}

bool EditSourceTexture::DeleteTextureFile() noexcept
{
    ReturnIfFalse(IsVaildTextureIndex());

    m_sourceBinder->RemoveKeyByFilename(m_textureFiles[m_texIndex]->GetFilename());
    RemoveTexture(m_texIndex);
    SelectTextureFile();

    return true;
}

void EditSourceTexture::SelectDefaultTextureFile() noexcept
{
    if (m_textureFiles.empty())
    {
        m_texIndex = -1;
        return;
    }

    m_texIndex = 0;
    SelectTextureFile();
}

bool EditSourceTexture::SelectTextureFile() noexcept
{
    auto curTex = IsVaildTextureIndex() ? m_textureFiles[m_texIndex].get() : nullptr;
    ApplyTexture(curTex);

    return true;
}

static vector<string> GetTextureFiles(const vector<unique_ptr<ImageGrid1>>& texFiles) noexcept
{
    vector<string> strList;
    for (auto& ws : texFiles)
        strList.emplace_back(WStringToString(ws->GetFilename()));
    return strList;
}

void EditSourceTexture::RenderTextureList()
{
    ImVec2 btnSize{ 130, 22 };
    if (ImGui::Button("Add Texture File", btnSize)) m_pendingAction = PendingAction::LoadTextureFile; ImGui::SameLine();
    if (ImGui::Button("Delete Texture File", btnSize)) m_pendingAction = PendingAction::DeleteTextureFile;

    auto strFileList = GetTextureFiles(m_textureFiles); //?!? 매프레임 실행될텐데...
    vector<const char*> texFiles; //const char*은 값을 저장하는게 아니라 포인터를 저장하기 때문에 strFileList가 없어지면 글자가 깨진다.
    for (auto& str : strFileList)
        texFiles.emplace_back(str.c_str());
    if (ImGui::ListBox("Texture List", &m_texIndex, texFiles.data(), static_cast<int>(texFiles.size()), 4))
        m_pendingAction = PendingAction::SelectTextureFile;
}