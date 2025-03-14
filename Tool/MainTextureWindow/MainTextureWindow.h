#pragma once
#include "../InnerWindow.h"

struct IRenderer;
class RenderTexture;
class ImageGrid1;
class MainSourceExtractor;
class EditTextureWindow;

class MainTextureWindow : public InnerWindow
{
    enum class SourcePartition
    {
        OnePart,
        ThreePartHorizontal,
        ThreePartVertical,
        NinePart
    };

public:
    ~MainTextureWindow();
    MainTextureWindow(IRenderer* renderer);
    virtual void Render(ImGuiIO* io) override;
    virtual bool SaveScene(const wstring& filename) override;
    virtual wstring GetSaveFilename() const noexcept override;
    
    bool Create(const wstring& filename); //Json파일
    bool CreateNew();
    bool LoadTexture(const wstring& filename);
    void Update();
    bool IsOpen() const noexcept { return m_isOpen; }
    void Open() noexcept { m_isOpen = true; }
    ImGuiWindow* GetWindow() noexcept { return m_window; }

private:
    ImVec2 GetWindowSize() const noexcept;

    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<RenderTexture> m_renderTex; //InnerWindow를 그리는 텍스쳐
    unique_ptr<ImageGrid1> m_sourceTexture; //작업할 텍스쳐
    unique_ptr<EditTextureWindow> m_editTextureWindow;
    ImVec2 m_size;
    bool m_isOpen{ false };

    //////////////////////////////////////////////////
private:
    Rectangle FindRectangleFromMousePosition() const noexcept;
    void SelectImageSource() noexcept;
    bool DeselectImageSource() noexcept;
    void CheckSourcePartition();
    void RenderPopupMenu();
    void RenderHighlightArea() const;

    SourcePartition m_sourcePartition{ SourcePartition::OnePart };
    vector<Rectangle> m_areaList;
    vector<Rectangle> m_hoveredAreas{};
    optional<vector<Rectangle>> m_selectAreas;
};

