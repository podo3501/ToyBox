#include "pch.h"
#include "SourceExtractor.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../../Utility.h"

SourceExtractor::~SourceExtractor() = default;
SourceExtractor::SourceExtractor() = default;

ImageGrid1Extractor::~ImageGrid1Extractor() = default;
ImageGrid1Extractor::ImageGrid1Extractor(ImageGrid1* imgGrid1) :
    m_component{ imgGrid1 }
{}

void ImageGrid1Extractor::Render(const ImGuiWindow* window) const
{
    
    DrawRectangle(m_component->Source, window);
}

unique_ptr<SourceExtractor> CreateSourceExtractor(UIComponent* component)
{
    if (!component) return nullptr;

    const string& type = component->GetType();
    if (type == "class ImageGrid1") return make_unique<ImageGrid1Extractor>(static_cast<ImageGrid1*>(component));

    return nullptr;
}