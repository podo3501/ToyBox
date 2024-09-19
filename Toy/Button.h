#pragma once

enum class ButtonState
{
    BT_Normal,
    BT_Over,
    BT_Click,
};

class Texture;

class Button
{
public:
    Button(ID3D12Device* device, DirectX::DescriptorHeap* descHeap);

    void Reset();
    void SetTexture(std::unique_ptr<Texture> normal, std::unique_ptr<Texture> over, std::unique_ptr<Texture> click);
    void SetTexture(ButtonState btnState, std::unique_ptr<Texture> tex);
    void Update(const DirectX::Mouse::State& state, const DirectX::SimpleMath::Vector2& pos);
    void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::SimpleMath::Vector2& screenPos);

private:
    void SetSize(Texture* tex);

    ID3D12Device* m_device;
    DirectX::DescriptorHeap* m_descHeap;
    DirectX::SimpleMath::Vector2 m_origin{ 0, 0 };
    ButtonState m_state{ ButtonState::BT_Normal };

    std::map<ButtonState, std::unique_ptr<Texture>> m_textures;
};

void SetButtonTexture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload,
    std::vector<std::tuple<int, std::wstring>>& filenames, Button* button);
