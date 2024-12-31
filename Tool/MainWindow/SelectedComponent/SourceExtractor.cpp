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
    m_filename{ filename }
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
        //���� �������� ���콺�� �ִ� ��ġ�� �ȼ��� ���´�
        IGetValue* value = GetRenderer()->GetValue();
        Microsoft::WRL::ComPtr<ID3D12Resource> readbackBuffer;
        value->GetReadTexture(GetResourceFullFilename(GetFilename()), readbackBuffer.GetAddressOf());

        //UINT8* data;
        //readbackBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

        //const XMINT2& pos = mouseTracker->GetOffsetPosition();
        //// �ȼ� ������ ��� (RGBA 4����Ʈ ����)
        //UINT pixelOffset = pos.y * 64 * 4 + pos.x * 4;
        //UINT32 pixelValue = *reinterpret_cast<UINT32*>(data + pixelOffset);

        //UINT8 r = (pixelValue >> 16) & 0xFF; // R ä�� (���� 8��Ʈ)
        //UINT8 g = (pixelValue >> 8) & 0xFF;  // G ä�� (���� 8��Ʈ)
        //UINT8 b = pixelValue & 0xFF;         // B ä�� (���� 8��Ʈ)
        //UINT8 a = (pixelValue >> 24) & 0xFF; // A ä�� (�ֻ��� 8��Ʈ)

        //readbackBuffer->Unmap(0, nullptr);
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