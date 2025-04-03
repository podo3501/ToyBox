#pragma once
#include "../InnerWindow.h"

struct IRenderer;
class RenderTexture;
class ImageGrid1;
class MainSourceExtractor;
class TextureSourceBinder;
class TexSrcCommandList;
class EditFontTexture;
class EditSourceTexture;

class TextureSourceWindow : public InnerWindow
{
public:
    ~TextureSourceWindow();
    TextureSourceWindow(IRenderer* renderer);
    virtual void Render(ImGuiIO* io) override;
    virtual bool SaveScene(const wstring& filename) override;
    virtual wstring GetSaveFilename() const noexcept override;
    
    bool Create(const wstring& filename); //Json����
    bool CreateNew();
    void SetTexture(ImageGrid1* texture) noexcept;
    void Update();
    bool IsOpen() const noexcept { return m_isOpen; }
    void Open() noexcept { m_isOpen = true; }
    ImGuiWindow* GetWindow() noexcept { return m_window; }

private:
    ImVec2 GetWindowSize() const noexcept;
    void RenderSourceWindow();

    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<TextureSourceBinder> m_sourceBinder;
    unique_ptr<TexSrcCommandList> m_cmdList;
    unique_ptr<RenderTexture> m_renderTex; //InnerWindow�� �׸��� �ؽ���
    ImageGrid1* m_sourceTexture; //�۾��� �ؽ���
    unique_ptr<EditFontTexture> m_editFontTexture;
    unique_ptr<EditSourceTexture> m_editSourceTexture;
    ImVec2 m_size;
    bool m_isOpen{ false };
};

