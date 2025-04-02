#pragma once

struct IRenderer;
class UIComponent;
class ImageGrid1;
class ImageGrid3;
class ImageGrid9;
class UICommandList;

class SourceExtractor
{
public:
    virtual ~SourceExtractor();
    SourceExtractor() = delete;

    void Update();
    void Render() const;

    inline void SetWindow(ImGuiWindow* texWnd) noexcept { m_window = texWnd; }
    inline void SetTextureAreaList(const vector<Rectangle>& areaList) noexcept { m_areaList = areaList; }

protected:
    SourceExtractor(IRenderer* renderer, const wstring& filename, UICommandList* cmdList) noexcept;
    Rectangle FindRectangleFromMousePosition() const noexcept;

    virtual void UpdateProcess() {};
    virtual void RenderProcess() const {};

    inline const wstring& GetFilename() const noexcept { return m_filename; }
    inline IRenderer* GetRenderer() const noexcept { return m_renderer; }
    inline ImGuiWindow* GetWindow() const noexcept { return m_window; }
    inline UICommandList* GetCommandList() const noexcept { return m_cmdList; }

private:
    IRenderer* m_renderer;
    wstring m_filename;
    UICommandList* m_cmdList;
    ImGuiWindow* m_window;

    vector<Rectangle> m_areaList;
};

class ImageGrid1Extractor : public SourceExtractor
{
public:
    ~ImageGrid1Extractor();
    ImageGrid1Extractor() = delete;
    ImageGrid1Extractor(IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1, UICommandList* cmdList) noexcept;

protected:
    virtual void UpdateProcess() override;
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
    ImageGrid3Extractor(IRenderer* renderer, const wstring& filename, ImageGrid3* imgGrid3, UICommandList* cmdList) noexcept;

protected:
    virtual void UpdateProcess() override;
    virtual void RenderProcess() const override;

private:
    ImageGrid3* m_component;
    vector<Rectangle> m_hoveredAreas;
};

class ImageGrid9Extractor : public SourceExtractor
{
public:
    ~ImageGrid9Extractor();
    ImageGrid9Extractor() = delete;
    ImageGrid9Extractor(IRenderer* renderer, const wstring& filename, ImageGrid9* imgGrid9, UICommandList* cmdList) noexcept;

protected:
    virtual void UpdateProcess() override;
    virtual void RenderProcess() const override;

private:
    ImageGrid9* m_component;
    vector<Rectangle> m_hoveredAreas;
};

unique_ptr<SourceExtractor> CreateSourceExtractor(IRenderer* renderer,
    const wstring& filename, UIComponent* component, UICommandList* cmdList);
