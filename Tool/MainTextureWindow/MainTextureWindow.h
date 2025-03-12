#pragma once

struct IRenderer;
class RenderTexture;
class ImageGrid1;
class MainSourceExtractor;
class EditTextureWindow;

class MainTextureWindow
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
    //MainTextureWindow(IRenderer* renderer, unique_ptr<MainSourceExtractor> sourceExtractor);
    MainTextureWindow(IRenderer* renderer);
    
    bool Create(const wstring& filename);
    void Update();
    void Render();
    bool IsOpen() const noexcept { return m_isOpen; }
    void Open() noexcept { m_isOpen = true; }

private:
    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<RenderTexture> m_renderTex;
    unique_ptr<ImageGrid1> m_sourceTexture;
    unique_ptr<EditTextureWindow> m_editTextureWindow;
    ImVec2 m_size;
    string m_name;
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

