#include "pch.h"
#include "SourceExtractor.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/InputManager.h"
#include "../../Utility.h"
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
SourceExtractor::SourceExtractor(IRenderer* renderer, const wstring& filename, CommandList* cmdList) noexcept :
    m_renderer{ renderer },
    m_filename{ filename },
    m_cmdList{ cmdList },
    m_window{ nullptr }
{}

Rectangle SourceExtractor::FindRectangleFromMousePosition() const noexcept
{
    const XMINT2& pos = InputManager::GetMouse().GetPosition();
    auto it = ranges::find_if(m_areaList, [&pos](const Rectangle& rect) {
        return Contains(rect, pos);
        });

    if (it != m_areaList.end()) {
        return *it;
    }

    return {};
}

void SourceExtractor::Update()
{
    ImGuiWindow* window = GetWindow();
    if (!IsWindowFocus(window)) return;

    UpdateProcess();
}

void SourceExtractor::Render() const
{
    RenderProcess();
}

////////////////////////////////////////////////////////////

ImageGrid1Extractor::~ImageGrid1Extractor() = default;
ImageGrid1Extractor::ImageGrid1Extractor( IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1, CommandList* cmdList) noexcept :
    SourceExtractor(renderer, filename, cmdList),
    m_component{ imgGrid1 }
{}

void ImageGrid1Extractor::UpdateProcess()
{
    m_hoveredArea = FindRectangleFromMousePosition();
    if(IsInputAction(MouseButton::Left, KeyState::Pressed))
    {
        if (m_hoveredArea != Rectangle{})
            GetCommandList()->SetSource(m_component, m_hoveredArea);
    }
}

void ImageGrid1Extractor::RenderProcess() const
{
    DrawRectangle(m_component->GetSource(), GetWindow());
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
static bool SetSourcesOnMouseClick(CommandList* cmdList, ImageGrid imageGrid3or9, const vector<Rectangle>& hoveredAreas)
{
    if (!IsInputAction(MouseButton::Left, KeyState::Pressed)) return true;
    if (hoveredAreas.empty()) return false;

    return cmdList->SetSources(imageGrid3or9, hoveredAreas);
}

static vector<Rectangle> GenerateSourceAreas(const Rectangle& area, bool is9Grid)
{
    vector<int> widths{};
    vector<int> heights = is9Grid ? vector<int>{} : vector<int>{ area.height };
    DivideLengthByThree(area, &widths, is9Grid ? &heights : nullptr);
    
    return GetSourcesFromArea(area, widths, heights);
}

ImageGrid3Extractor::~ImageGrid3Extractor() = default;
ImageGrid3Extractor::ImageGrid3Extractor(IRenderer* renderer, const wstring& filename, ImageGrid3* imgGrid3, CommandList* cmdList) noexcept :
    SourceExtractor(renderer, filename, cmdList),
    m_component{ imgGrid3 }
{}

void ImageGrid3Extractor::UpdateProcess()
{
    m_hoveredAreas = GenerateSourceAreas(FindRectangleFromMousePosition(), false);
    SetSourcesOnMouseClick(GetCommandList(), m_component, m_hoveredAreas);
}

void ImageGrid3Extractor::RenderProcess() const
{
    DrawRectangle(m_component->GetSources(), GetWindow());
    DrawRectangle(m_hoveredAreas, GetWindow());
}

////////////////////////////////////////////////////////////

ImageGrid9Extractor::~ImageGrid9Extractor() = default;
ImageGrid9Extractor::ImageGrid9Extractor(IRenderer* renderer, const wstring& filename, ImageGrid9* imgGrid9, CommandList* cmdList) noexcept :
    SourceExtractor(renderer, filename, cmdList),
    m_component{ imgGrid9 }
{}

void ImageGrid9Extractor::UpdateProcess()
{
    m_hoveredAreas = GenerateSourceAreas(FindRectangleFromMousePosition(), true);
    SetSourcesOnMouseClick(GetCommandList(), m_component, m_hoveredAreas);
}

void ImageGrid9Extractor::RenderProcess() const
{
    DrawRectangle(m_component->GetSources(), GetWindow());
    DrawRectangle(m_hoveredAreas, GetWindow());
}

////////////////////////////////////////////////////////////

unique_ptr<SourceExtractor> CreateSourceExtractor(IRenderer* renderer,
    const wstring& filename, UIComponent* component, CommandList* cmdList)
{
    if (!component) return nullptr;
    
    ComponentID id = component->GetTypeID();
    if (id == ComponentID::ImageGrid1) 
        return make_unique<ImageGrid1Extractor>(renderer, filename, ComponentCast<ImageGrid1*>(component), cmdList);
    if (id == ComponentID::ImageGrid3)
        return make_unique<ImageGrid3Extractor>(renderer, filename, ComponentCast<ImageGrid3*>(component), cmdList);
    if (id == ComponentID::ImageGrid9)
        return make_unique<ImageGrid9Extractor>(renderer, filename, ComponentCast<ImageGrid9*>(component), cmdList);

    return nullptr;
}