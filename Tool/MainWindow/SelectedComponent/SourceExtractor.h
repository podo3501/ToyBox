#pragma once

class UIComponent;
class ImageGrid1;

class SourceExtractor
{
public:
    virtual ~SourceExtractor();
    SourceExtractor();

    virtual void Render(const ImGuiWindow* window) const {};

private:

};

class ImageGrid1Extractor : public SourceExtractor
{
public:
    ~ImageGrid1Extractor();
    ImageGrid1Extractor(ImageGrid1* imgGrid1);

    virtual void Render(const ImGuiWindow* window) const override;

private:
    ImageGrid1* m_component;
};

unique_ptr<SourceExtractor> CreateSourceExtractor(UIComponent* component);
