#pragma once

struct IRenderer;
class UIComponent;
class InputManager;
class ImageGrid1;

class SourceExtractor
{
public:
    virtual ~SourceExtractor();
    SourceExtractor() = delete;

    virtual bool Initialize() { return true; };
    virtual void Update(InputManager* inputManager) {};
    virtual void Render() const {};

    inline void SetWindow(ImGuiWindow* texWnd) noexcept { m_window = texWnd; }

protected:
    SourceExtractor(IRenderer* renderer, const wstring& filename) noexcept;
    inline const wstring& GetFilename() const noexcept { return m_filename; }
    inline IRenderer* GetRenderer() const noexcept { return m_renderer; }
    inline ImGuiWindow* GetWindow() const noexcept { return m_window; }

private:
    IRenderer* m_renderer;
    wstring m_filename;
    ImGuiWindow* m_window;
};

class ImageGrid1Extractor : public SourceExtractor
{
public:
    ~ImageGrid1Extractor();
    ImageGrid1Extractor(IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1) noexcept;

    virtual bool Initialize() override;
    virtual void Update(InputManager* inputManager) override;
    virtual void Render() const override;

private:
    Rectangle FindRectangleContainingPoint(const XMINT2& pos);

    ImageGrid1* m_component;
    bool m_IsInitialize{ false };

    vector<Rectangle> m_areaList;
    Rectangle m_hoveredRect{};
};

unique_ptr<SourceExtractor> CreateSourceExtractor(IRenderer* renderer, const wstring& filename, UIComponent* component);
