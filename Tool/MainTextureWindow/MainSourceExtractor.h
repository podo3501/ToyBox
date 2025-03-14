#pragma once

struct IRenderer;
class UIComponent;
class ImageGrid1;
class ImageGrid3;
class ImageGrid9;
class CommandList;

class MainSourceExtractor
{
public:
    virtual ~MainSourceExtractor();
    MainSourceExtractor() = delete;

    void Update();
    void Render() const;

    inline void SetWindow(ImGuiWindow* texWnd) noexcept { m_window = texWnd; }
    inline void SetTextureAreaList(const vector<Rectangle>& areaList) noexcept { m_areaList = areaList; }

protected:
    MainSourceExtractor(IRenderer* renderer, const wstring& filename, CommandList* cmdList) noexcept;
    Rectangle FindRectangleFromMousePosition() const noexcept;

    virtual void UpdateProcess() {};
    virtual void RenderProcess() const {};

    inline const wstring& GetFilename() const noexcept { return m_filename; }
    inline IRenderer* GetRenderer() const noexcept { return m_renderer; }
    inline ImGuiWindow* GetWindow() const noexcept { return m_window; }
    inline CommandList* GetCommandList() const noexcept { return m_cmdList; }

private:
    IRenderer* m_renderer;
    wstring m_filename;
    CommandList* m_cmdList;
    ImGuiWindow* m_window;

    vector<Rectangle> m_areaList;
};

//class ImageGrid1Extractor : public MainSourceExtractor
//{
//public:
//    ~ImageGrid1Extractor();
//    ImageGrid1Extractor() = delete;
//    ImageGrid1Extractor(IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1, CommandList* cmdList) noexcept;
//
//protected:
//    virtual void UpdateProcess() override;
//    virtual void RenderProcess() const override;
//
//private:
//    ImageGrid1* m_component;
//    Rectangle m_hoveredArea{};
//};
//
//class ImageGrid3Extractor : public MainSourceExtractor
//{
//public:
//    ~ImageGrid3Extractor();
//    ImageGrid3Extractor() = delete;
//    ImageGrid3Extractor(IRenderer* renderer, const wstring& filename, ImageGrid3* imgGrid3, CommandList* cmdList) noexcept;
//
//protected:
//    virtual void UpdateProcess() override;
//    virtual void RenderProcess() const override;
//
//private:
//    ImageGrid3* m_component;
//    vector<Rectangle> m_hoveredAreas;
//};
//
//class ImageGrid9Extractor : public MainSourceExtractor
//{
//public:
//    ~ImageGrid9Extractor();
//    ImageGrid9Extractor() = delete;
//    ImageGrid9Extractor(IRenderer* renderer, const wstring& filename, ImageGrid9* imgGrid9, CommandList* cmdList) noexcept;
//
//protected:
//    virtual void UpdateProcess() override;
//    virtual void RenderProcess() const override;
//
//private:
//    ImageGrid9* m_component;
//    vector<Rectangle> m_hoveredAreas;
//};

//unique_ptr<MainSourceExtractor> CreateMainSourceExtractor(IRenderer* renderer,
//    const wstring& filename, UIComponent* component, CommandList* cmdList);
