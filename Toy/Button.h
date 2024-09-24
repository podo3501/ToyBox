#pragma once
#include "../Include/IRenderItem.h"

enum class ButtonState
{
    BT_Normal,
    BT_Over,
    BT_Click,
};

class Texture;

class Button : public IRenderItem
{
public:
    Button(const std::wstring& resPath, int width, int height);
    virtual ~Button();

    virtual void OnDeviceLost() override;
    virtual void LoadResources(ID3D12Device* device,
        DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) override;
    virtual void Render(DirectX::DX12::SpriteBatch* sprite) override;

    void SetTexture(std::unique_ptr<Texture> normal, std::unique_ptr<Texture> over, std::unique_ptr<Texture> click);
    void SetTexture(ButtonState btnState, std::unique_ptr<Texture> tex);
    void Update(const DirectX::Mouse::State& state);

private:
    void Reset();
    void SetSize(Texture* tex);

    std::wstring m_resPath{};
    std::map<ButtonState, std::unique_ptr<Texture>> m_textures;

    DirectX::SimpleMath::Vector2 m_screenPos{ 0, 0 };
    DirectX::SimpleMath::Vector2 m_origin{ 0, 0 };
    ButtonState m_state{ ButtonState::BT_Normal };
};

void SetButtonTexture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload,
    std::vector<std::tuple<int, std::wstring>>& filenames, Button* button);
