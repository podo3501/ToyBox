#pragma once
#include "../Toy/UserInterface/TextureSourceBinder.h"

enum class ImagePart;
enum class PendingAction;
struct IRenderer;
class ImageGrid1;
struct TextureSourceInfo;
class TextureSourceBinder;
class MainTextureWindow;
class RenameNotifier;
class ImageSelector;

class EditTextureWindow
{
public:
    ~EditTextureWindow();
    EditTextureWindow(IRenderer* renderer, MainTextureWindow* textureWindow);

    void Update() noexcept;
    void Render();
    bool SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept;

private:
    inline bool IsVaildTextureIndex() const noexcept { return m_texIndex >= 0 && m_texIndex < m_textureFiles.size(); }
    bool IsLoadedTexture(const wstring& filename) const noexcept;
    bool LoadTextureFromFile(const wstring& filename);
    bool LoadTextureFile();
    void AddTexture(unique_ptr<ImageGrid1> texture) noexcept;
    void RemoveTexture(int textureIdx) noexcept;
    void ApplyTexture(ImageGrid1* tex) const noexcept;
    bool DeleteTextureFile() noexcept;
    void SelectDefaultTextureFile() noexcept;
    bool SelectTextureFile() noexcept;
    bool ExecuteAction() noexcept;
    void RenderTextureList();
    
    IRenderer* m_renderer;
    MainTextureWindow* m_textureWindow;
    TextureSourceBinder* m_sourceBinder;
    unique_ptr<ImageSelector> m_imageSelector;
    int m_texIndex{ -1 };
    vector<unique_ptr<ImageGrid1>> m_textureFiles;
    optional<PendingAction> m_pendingAction;
};