#pragma once

struct IRenderer;
class UIComponent;
class InputManager;
class ImageGrid1;
class ImageGrid3;

class SourceExtractor
{
public:
    virtual ~SourceExtractor();
    SourceExtractor() = delete;

    virtual bool Initialize();

    void Update(InputManager* inputManager);
    void Render() const;

    inline void SetWindow(ImGuiWindow* texWnd) noexcept { m_window = texWnd; }

protected:
    SourceExtractor(IRenderer* renderer, const wstring& filename) noexcept;
    Rectangle FindRectangleContainingPoint(const XMINT2& pos);

    virtual void UpdateProcess(InputManager* inputManager) {};
    virtual void RenderProcess() const {};

    inline const wstring& GetFilename() const noexcept { return m_filename; }
    inline IRenderer* GetRenderer() const noexcept { return m_renderer; }
    inline ImGuiWindow* GetWindow() const noexcept { return m_window; }

private:
    IRenderer* m_renderer;
    wstring m_filename;
    ImGuiWindow* m_window;

    vector<Rectangle> m_areaList;
    bool m_IsInitialize{ false };
};

class ImageGrid1Extractor : public SourceExtractor
{
public:
    ~ImageGrid1Extractor();
    ImageGrid1Extractor() = delete;
    ImageGrid1Extractor(IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1) noexcept;

protected:
    virtual void UpdateProcess(InputManager* inputManager) override;
    virtual void RenderProcess() const override;

private:
    ImageGrid1* m_component;
    Rectangle m_hoveredArea{};
};

class ImageGrid3Extractor : public SourceExtractor
{
public:
    ~ImageGrid3Extractor();
    ImageGrid3Extractor() = delete;
    ImageGrid3Extractor(IRenderer* renderer, const wstring& filename, ImageGrid3* imgGrid3) noexcept;

protected:
    virtual void UpdateProcess(InputManager* inputManager) override;
    virtual void RenderProcess() const override;

private:
    ImageGrid3* m_component;
    vector<Rectangle> m_hoveredAreas;
};

unique_ptr<SourceExtractor> CreateSourceExtractor(IRenderer* renderer, const wstring& filename, UIComponent* component);
