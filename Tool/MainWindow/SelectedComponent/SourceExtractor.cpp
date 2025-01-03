#include "pch.h"
#include "SourceExtractor.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/InputManager.h"
#include "../../Utility.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"

SourceExtractor::~SourceExtractor() = default;
SourceExtractor::SourceExtractor(IRenderer* renderer, const wstring& filename) noexcept :
    m_renderer{ renderer },
    m_filename{ filename },
    m_window{ nullptr }
{}

////////////////////////////////////////////////////////////

static inline UINT32 PackRGBA(UINT8 r, UINT8 g, UINT8 b, UINT8 a) {
    return (static_cast<UINT32>(a) << 24) |
        (static_cast<UINT32>(b) << 16) |
        (static_cast<UINT32>(g) << 8) |
        (static_cast<UINT32>(r));
}

ImageGrid1Extractor::~ImageGrid1Extractor() = default;
ImageGrid1Extractor::ImageGrid1Extractor( IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1) noexcept :
    SourceExtractor(renderer, filename),
    m_component{ imgGrid1 }
{
}

bool ImageGrid1Extractor::Initialize()
{
    IGetValue* value = GetRenderer()->GetValue();
    ReturnIfFalse(value->GetTextureAreaList(GetResourceFullFilename(GetFilename()), PackRGBA(255, 255, 255, 0), m_areaList));
    m_IsInitialize = true;
    return true;
}

Rectangle ImageGrid1Extractor::FindRectangleContainingPoint(const XMINT2& pos)
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

void ImageGrid1Extractor::Update(InputManager* inputManager)
{
    ImGuiWindow* window = GetWindow();
    if (!IsWindowFocus(window)) return;

    if (!m_IsInitialize)
        Initialize();

    auto mouseTracker = inputManager->GetMouse();
    const XMINT2& pos = mouseTracker->GetOffsetPosition();
    m_hoveredRect = FindRectangleContainingPoint(pos);
    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
    {
        if (m_hoveredRect != Rectangle{})
            m_component->Source = m_hoveredRect;
    }
}

void ImageGrid1Extractor::Render() const
{
    DrawRectangle(m_component->Source, GetWindow());
    DrawRectangle(m_hoveredRect, GetWindow());
}

////////////////////////////////////////////////////////////

unique_ptr<SourceExtractor> CreateSourceExtractor(
    IRenderer* renderer, const wstring& filename, UIComponent* component)
{
    if (!component) return nullptr;
    
    const string& type = component->GetType();
    if (type == "class ImageGrid1") 
        return make_unique<ImageGrid1Extractor>(renderer, filename, ComponentCast<ImageGrid1*>(component));

    return nullptr;
}