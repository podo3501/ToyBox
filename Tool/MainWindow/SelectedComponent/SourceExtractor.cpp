#include "pch.h"
#include "SourceExtractor.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"
#include "../Toy/InputManager.h"
#include "../../Utility.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIUtility.h"

static inline UINT32 PackRGBA(UINT8 r, UINT8 g, UINT8 b, UINT8 a) 
{
    return (static_cast<UINT32>(a) << 24) |
        (static_cast<UINT32>(b) << 16) |
        (static_cast<UINT32>(g) << 8) |
        (static_cast<UINT32>(r));
}

SourceExtractor::~SourceExtractor() = default;
SourceExtractor::SourceExtractor(IRenderer* renderer, const wstring& filename) noexcept :
    m_renderer{ renderer },
    m_filename{ filename },
    m_window{ nullptr }
{}

Rectangle SourceExtractor::FindRectangleContainingPoint(const XMINT2& pos)
{
    auto it = ranges::find_if(m_areaList, [&pos](const Rectangle& rect) {
        return
            pos.x >= rect.x &&
            pos.x <= rect.x + rect.width &&
            pos.y >= rect.y &&
            pos.y <= rect.y + rect.height;
        });

    if (it != m_areaList.end()) {
        return *it;
    }

    return {};
}

bool SourceExtractor::Initialize()
{
    IGetValue* value = m_renderer->GetValue();
    ReturnIfFalse(value->GetTextureAreaList(GetResourceFullFilename(m_filename), PackRGBA(255, 255, 255, 0), m_areaList));
    m_IsInitialize = true;
    return true;
}

void SourceExtractor::Update(InputManager* inputManager)
{
    ImGuiWindow* window = GetWindow();
    if (!IsWindowFocus(window)) return;

    if (!m_IsInitialize)
        Initialize();

    UpdateProcess(inputManager);
}

void SourceExtractor::Render() const
{
    RenderProcess();
}

////////////////////////////////////////////////////////////

ImageGrid1Extractor::~ImageGrid1Extractor() = default;
ImageGrid1Extractor::ImageGrid1Extractor( IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1) noexcept :
    SourceExtractor(renderer, filename),
    m_component{ imgGrid1 }
{}

void ImageGrid1Extractor::UpdateProcess(InputManager* inputManager)
{
    auto mouseTracker = inputManager->GetMouse();
    const XMINT2& pos = mouseTracker->GetOffsetPosition();
    m_hoveredArea = FindRectangleContainingPoint(pos);
    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
    {
        if (m_hoveredArea != Rectangle{})
            m_component->Source = m_hoveredArea;
    }
}

void ImageGrid1Extractor::RenderProcess() const
{
    DrawRectangle(m_component->Source, GetWindow());
    DrawRectangle(m_hoveredArea, GetWindow());
}

////////////////////////////////////////////////////////////

ImageGrid3Extractor::~ImageGrid3Extractor() = default;
ImageGrid3Extractor::ImageGrid3Extractor(IRenderer* renderer, const wstring& filename, ImageGrid3* imgGrid3) noexcept :
    SourceExtractor(renderer, filename),
    m_component{ imgGrid3 }
{}

vector<int> DivideWidthByThree(const Rectangle& area)
{
    if (area.IsEmpty()) return {};

    int oneThirdWidth = area.width / 3;

    vector<int> widths;
    widths.emplace_back(oneThirdWidth);
    widths.emplace_back(area.width - oneThirdWidth * 2);
    widths.emplace_back(oneThirdWidth);

    return widths;
}

void ImageGrid3Extractor::UpdateProcess(InputManager* inputManager)
{
    auto mouseTracker = inputManager->GetMouse();
    const XMINT2& pos = mouseTracker->GetOffsetPosition();
    Rectangle area = FindRectangleContainingPoint(pos);

    vector<int> widths = DivideWidthByThree(area);
    m_hoveredAreas = GetSourcesFromAreaAndGaps(area, widths);
    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
    {
        if (!m_hoveredAreas.empty())
            m_component->SetSources(m_hoveredAreas);
    }
}

void ImageGrid3Extractor::RenderProcess() const
{
    ranges::for_each(m_component->GetSources(), [this](const auto& area) { DrawRectangle(area, GetWindow()); });
    ranges::for_each(m_hoveredAreas, [this](const auto& area) { DrawRectangle(area, GetWindow()); });
}

////////////////////////////////////////////////////////////

unique_ptr<SourceExtractor> CreateSourceExtractor(
    IRenderer* renderer, const wstring& filename, UIComponent* component)
{
    if (!component) return nullptr;
    
    const string& type = component->GetType();
    if (type == "class ImageGrid1") 
        return make_unique<ImageGrid1Extractor>(renderer, filename, ComponentCast<ImageGrid1*>(component));
    if (type == "class ImageGrid3")
        return make_unique<ImageGrid3Extractor>(renderer, filename, ComponentCast<ImageGrid3*>(component));

    return nullptr;
}