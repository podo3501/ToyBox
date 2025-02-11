#pragma once

struct IRenderer;
class ImageGrid1;
class SourceExtractor;

class TextureWindow
{
public:
    ~TextureWindow();
    TextureWindow(IRenderer* renderer, unique_ptr<SourceExtractor> sourceExtractor);
    
    bool Create(const wstring& filename);
    void Update();
    void Render();
    bool IsOpen() const noexcept { return m_isOpen; }
    void Open() noexcept { m_isOpen = true; }

private:
    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<SourceExtractor> m_sourceExtractor;
    unique_ptr<ImageGrid1> m_sourceTexture;

    ImTextureID m_textureID{};
    ImVec2 m_size;
    string m_name;
    bool m_isOpen{ false };
};

