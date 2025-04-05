#pragma once

enum class PendingAction;
struct IRenderer;
class ImageGrid1;
class TextureLoadBinder;
class TexResCommandList;
class TextureResBinderWindow;
class EditListBox;
class RenameNotifier;
class ImageSelector;

class EditSourceTexture
{
public:
    ~EditSourceTexture();
    EditSourceTexture(IRenderer* renderer, TextureResBinderWindow* textureWindow);

    void Update() noexcept;
    void Render();
    bool SetCommandList(TexResCommandList* cmdList) noexcept;
    void CheckTextureByUndoRedo();

private:
    inline bool IsVaildTextureIndex() const noexcept { return m_texIndex >= 0 && m_texIndex < m_textureFiles.size(); }
    bool IsLoadedTexture(const wstring& filename) const noexcept;
    bool LoadTextureFromFile(const wstring& filename);
    bool LoadTextureFile();
    void AddTexture(unique_ptr<ImageGrid1> texture) noexcept;
    void RemoveTexture(int textureIdx) noexcept;
    void RemoveTexture(const wstring& filename) noexcept;
    void ApplyTexture(ImageGrid1* tex) const noexcept;
    bool DeleteTextureFile() noexcept;
    void SelectDefaultTextureFile() noexcept;
    bool SelectTextureFile() noexcept;
    bool ExecuteAction() noexcept;
    void RenderTextureList();
    
    IRenderer* m_renderer;
    TextureResBinderWindow* m_textureWindow;
    unique_ptr<TextureLoadBinder> m_textureLoader;
    TexResCommandList* m_cmdList;
    unique_ptr<ImageSelector> m_imageSelector;
    int m_texIndex{ -1 };
    unique_ptr<EditListBox> m_listboxTexture;
    vector<unique_ptr<ImageGrid1>> m_textureFiles;
    optional<PendingAction> m_pendingAction;
};