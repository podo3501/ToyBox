#include "pch.h"
#include "Button.h"
#include "Texture.h"

using namespace DirectX;

Button::Button(ID3D12Device* device, DirectX::DescriptorHeap* descHeap) :
    m_device{ device }, m_descHeap{ descHeap } {}

void Button::SetTexture(std::unique_ptr<Texture> normal, std::unique_ptr<Texture> over, std::unique_ptr<Texture> click)
{
    SetSize(normal.get());

    m_textures.insert(std::make_pair(ButtonState::BT_Normal, std::move(normal)));
    m_textures.insert(std::make_pair(ButtonState::BT_Over, std::move(over)));
    m_textures.insert(std::make_pair(ButtonState::BT_Click, std::move(click)));
}

void Button::SetTexture(ButtonState btnState, std::unique_ptr<Texture> tex)
{
    if (btnState == ButtonState::BT_Normal)
        SetSize(tex.get());

    m_textures.insert(std::make_pair(btnState, std::move(tex)));
}

void Button::SetSize(Texture* tex)
{
    XMUINT2 size = tex->GetSize();

    m_origin.x = float(size.x / 2);
    m_origin.y = float(size.y / 2);
}

void Button::Reset()
{
    std::ranges::for_each(m_textures | std::views::values, [](auto& tex) {
        tex->Reset();
        });
}

void Button::Update(const Mouse::State& state, const DirectX::SimpleMath::Vector2& pos)
{
    int x = state.x - static_cast<int>(pos.x);
    int y = state.y - static_cast<int>(pos.y);

    m_state = ButtonState::BT_Normal;

    if ((-m_origin.x < x && x < m_origin.x) && (-m_origin.y < y && y < m_origin.y))
        m_state = ButtonState::BT_Over;

    if (m_state == ButtonState::BT_Over && state.leftButton)
        m_state = ButtonState::BT_Click;
}

void Button::Render(DirectX::SpriteBatch* spriteBatch, const DirectX::SimpleMath::Vector2& screenPos)
{
    m_textures[m_state]->Draw(spriteBatch, screenPos);
}

///////////////////////////////////////////////////////////////////////////////////
void SetButtonTexture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, ResourceUploadBatch& resUpload,
    std::vector<std::tuple<int, std::wstring>>& filenames, Button* button)
{
    std::ranges::for_each(filenames, [device, descHeap, &resUpload, bs{ ButtonState::BT_Normal }, button](auto& file) mutable {
        std::unique_ptr<Texture> tex = std::make_unique<Texture>(device, descHeap);
        tex->Upload(resUpload, std::get<0>(file), std::get<1>(file));

        button->SetTexture(bs, std::move(tex));
        bs = static_cast<ButtonState>(static_cast<int>(bs) + 1);
        });
}