#pragma once

#include "../Include/IRenderItem.h"

enum class ButtonStateOld
{
    BT_Normal,
    BT_Over,
    BT_Click,
};

class Texture;

class Button : public IRenderItem
{
public:
    Button(const std::wstring& resPath, const DirectX::SimpleMath::Vector2& pos);
    virtual ~Button();

    virtual void OnDeviceLost() override;
    virtual void LoadResources(ID3D12Device* device,
        DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) override;
    virtual void Render(DirectX::DX12::SpriteBatch* sprite, const DirectX::SimpleMath::Vector2& outputSize) override;

    void SetTexture(std::unique_ptr<Texture> normal, std::unique_ptr<Texture> over, std::unique_ptr<Texture> click);
    void SetTexture(ButtonStateOld btnState, std::unique_ptr<Texture> tex);
    void Update(const DirectX::SimpleMath::Vector2& resolution, const DirectX::Mouse::State& state);

private:
    void Reset();
    void SetSize(Texture* tex);

    std::wstring m_resPath{};
    std::map<ButtonStateOld, std::unique_ptr<Texture>> m_textures;

    DirectX::SimpleMath::Vector2 m_screenPos{ 0, 0 };
    DirectX::SimpleMath::Vector2 m_origin{ 0, 0 };
    ButtonStateOld m_state{ ButtonStateOld::BT_Normal };
};

void SetButtonTexture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload,
    std::vector<std::tuple<int, std::wstring>>& filenames, Button* button);
