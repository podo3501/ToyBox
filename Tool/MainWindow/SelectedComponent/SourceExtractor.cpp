#include "pch.h"
#include "SourceExtractor.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/InputManager.h"
#include "../../Utility.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Config.h"

SourceExtractor::~SourceExtractor() = default;
SourceExtractor::SourceExtractor(IRenderer* renderer, const wstring& filename) noexcept :
    m_renderer{ renderer },
    m_filename{ filename },
    m_window{ nullptr }
{}

////////////////////////////////////////////////////////////

ImageGrid1Extractor::~ImageGrid1Extractor() = default;
ImageGrid1Extractor::ImageGrid1Extractor( IRenderer* renderer, const wstring& filename, ImageGrid1* imgGrid1) noexcept :
    SourceExtractor(renderer, filename),
    m_component{ imgGrid1 }
{}

void ImageGrid1Extractor::Update(InputManager* inputManager)
{
    ImGuiWindow* window = GetWindow();
    if (!IsWindowFocus(window)) return;

    auto mouseTracker = inputManager->GetMouse();
    if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
    {
        //현재 윈도우의 마우스가 있는 위치의 픽셀을 얻어온다
        IGetValue* value = GetRenderer()->GetValue();
        vector<Rectangle> areaList;
        //value->GetTextureAreaList(GetResourceFullFilename(GetFilename()), areaList);
        Microsoft::WRL::ComPtr<ID3D12Resource> readbackBuffer;
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
        value->GetReadTexture(GetResourceFullFilename(GetFilename()), readbackBuffer.GetAddressOf(), &layout);

        UINT8* data{ nullptr };
        readbackBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

        const XMINT2& pos = mouseTracker->GetOffsetPosition();
        // 픽셀 오프셋 계산 (RGBA 4바이트 기준)
        UINT pixelOffset = pos.y * layout.Footprint.RowPitch + pos.x * 4;
        UINT32 pixelValue = *reinterpret_cast<UINT32*>(data + pixelOffset);

        UINT8 red = (pixelValue >> 0) & 0xFF;    // R 채널 (하위 8비트)
        UINT8 green = (pixelValue >> 8) & 0xFF;  // G 채널 (다음 8비트)
        UINT8 blue = (pixelValue >> 16) & 0xFF;  // B 채널 (상위 8비트)
        UINT8 alpha = (pixelValue >> 24) & 0xFF; // A 채널 (최상위 8비트)

        readbackBuffer->Unmap(0, nullptr);
    }
}

void ImageGrid1Extractor::Render() const
{
    DrawRectangle(m_component->Source, GetWindow());
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