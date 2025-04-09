#pragma once
#include "../InnerWindow.h"

struct IRenderer;
class RenderTexture;
class PatchTexture1;
class MainSourceExtractor;
class TextureResourceBinder;
class TexResCommandList;
class EditFontTexture;
class EditSourceTexture;

class TextureResBinderWindow : public InnerWindow
{
public:
    ~TextureResBinderWindow();
    TextureResBinderWindow(IRenderer* renderer);
    virtual void Render(ImGuiIO* io) override;
    virtual bool SaveScene(const wstring& filename) override;
    virtual wstring GetSaveFilename() const noexcept override;
    
    bool Create(const wstring& filename = L""); //Json����
    void SetTexture(PatchTexture1* pTex1) noexcept;
    void Update();
    bool IsOpen() const noexcept { return m_isOpen; }
    void Open() noexcept { m_isOpen = true; }
    ImGuiWindow* GetWindow() noexcept { return m_window; }

private:
    bool CheckUndoRedo();
    ImVec2 GetWindowSize() const noexcept;
    void RenderResourceWindow();

    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<TextureResourceBinder> m_resBinder;
    unique_ptr<TexResCommandList> m_cmdList;
    unique_ptr<RenderTexture> m_renderTex; //InnerWindow�� �׸��� �ؽ���
    PatchTexture1* m_sourceTexture; //�۾��� �ؽ���
    unique_ptr<EditFontTexture> m_editFontTexture;
    unique_ptr<EditSourceTexture> m_editSourceTexture;
    ImVec2 m_size;
    bool m_isOpen{ false };
};

