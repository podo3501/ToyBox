#pragma once

struct IRenderer;
class ExtractTexture;
class UIComponent;
class SourceExtractor;

class TextureWindow
{
public:
    ~TextureWindow();
    TextureWindow(IRenderer* renderer, unique_ptr<SourceExtractor> sourceExtractor);
    
    bool Create(const wstring& filename);
    void Render();
    bool IsOpen() const noexcept { return m_isOpen; }

private:
    IRenderer* m_renderer;
    unique_ptr<SourceExtractor> m_sourceExtractor;
    unique_ptr<ExtractTexture> m_extractTexture;
    ImTextureID m_textureID{};
    ImVec2 m_size;
    string m_name;
    bool m_isOpen{ false };
};

