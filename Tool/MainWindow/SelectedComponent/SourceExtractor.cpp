#include "pch.h"
#include "SourceExtractor.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"
#include "../Toy/UserInterface/ImageGrid9.h"
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

Rectangle SourceExtractor::FindRectangleContainingPoint(const XMINT2& pos) noexcept
{
    auto it = ranges::find_if(m_areaList, [&pos](const Rectangle& rect) {
        return Contains(rect, pos);
        });

    if (it != m_areaList.end()) {
        return *it;
    }

    return {};
}

Rectangle SourceExtractor::FindAreaFromMousePosition(InputManager* inputManager) noexcept
{
    auto mouseTracker = inputManager->GetMouse();
    const XMINT2& pos = mouseTracker->GetOffsetPosition();
    return FindRectangleContainingPoint(pos);
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
    m_hoveredArea = FindAreaFromMousePosition(inputManager);
    if (inputManager->GetMouse()->leftButton == Mouse::ButtonStateTracker::PRESSED)
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

static void DivideLengthByThree(const Rectangle& area, vector<int>* outWidth, vector<int>* outHeight)
{
    if (area.IsEmpty()) return;

    auto divideByThree = [](int totalSize) -> std::vector<int> {
        int oneThird = totalSize / 3;
        return { oneThird, totalSize - oneThird * 2, oneThird };
        };

    if (outWidth)
        (*outWidth) = divideByThree(area.width);
    if (outHeight)
        (*outHeight) = divideByThree(area.height);
}

template<typename ImageGrid>
static void SetSourcesOnMouseClick(InputManager* inputManager, 
    ImageGrid imageGrid3or9,
    const vector<Rectangle>& hoveredAreas)
{
    auto mouseTracker = inputManager->GetMouse();
    if (mouseTracker->leftButton != Mouse::ButtonStateTracker::PRESSED) return;
    if (hoveredAreas.empty()) return;

    imageGrid3or9->SetSources(hoveredAreas);
}

static vector<Rectangle> GenerateSourceAreas(const Rectangle& area, bool is9Grid)
{
    vector<int> widths{};
    vector<int> heights = is9Grid ? vector<int>{} : vector<int>{ area.height };
    DivideLengthByThree(area, &widths, is9Grid ? &heights : nullptr);
    
    return GetSourcesFromArea(area, widths, heights);
}

ImageGrid3Extractor::~ImageGrid3Extractor() = default;
ImageGrid3Extractor::ImageGrid3Extractor(IRenderer* renderer, const wstring& filename, ImageGrid3* imgGrid3) noexcept :
    SourceExtractor(renderer, filename),
    m_component{ imgGrid3 }
{}

void ImageGrid3Extractor::UpdateProcess(InputManager* inputManager)
{
    m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePosition(inputManager), false);
    SetSourcesOnMouseClick(inputManager, m_component, m_hoveredAreas);
}

void ImageGrid3Extractor::RenderProcess() const
{
    DrawRectangle(m_component->GetSources(), GetWindow());
    DrawRectangle(m_hoveredAreas, GetWindow());
}

////////////////////////////////////////////////////////////

ImageGrid9Extractor::~ImageGrid9Extractor() = default;
ImageGrid9Extractor::ImageGrid9Extractor(IRenderer* renderer, const wstring& filename, ImageGrid9* imgGrid9) noexcept :
    SourceExtractor(renderer, filename),
    m_component{ imgGrid9 }
{}

void ImageGrid9Extractor::UpdateProcess(InputManager* inputManager)
{
    m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePosition(inputManager), true);
    SetSourcesOnMouseClick(inputManager, m_component, m_hoveredAreas);
}

void ImageGrid9Extractor::RenderProcess() const
{
    DrawRectangle(m_component->GetSources(), GetWindow());
    DrawRectangle(m_hoveredAreas, GetWindow());
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
    if (type == "class ImageGrid9")
        return make_unique<ImageGrid9Extractor>(renderer, filename, ComponentCast<ImageGrid9*>(component));

    return nullptr;
}