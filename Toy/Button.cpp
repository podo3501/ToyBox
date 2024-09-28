#include "pch.h"
#include "Button.h"
#include "Texture.h"

using namespace DirectX;

Button::Button(const std::wstring& resPath, const SimpleMath::Vector2& pos) :
    m_resPath{ resPath }, m_screenPos{ pos }
{}
Button::~Button() = default;

void Button::SetTexture(std::unique_ptr<Texture> normal, std::unique_ptr<Texture> over, std::unique_ptr<Texture> click)
{
    SetSize(normal.get());

    m_textures.insert(std::make_pair(ButtonStateOld::BT_Normal, std::move(normal)));
    m_textures.insert(std::make_pair(ButtonStateOld::BT_Over, std::move(over)));
    m_textures.insert(std::make_pair(ButtonStateOld::BT_Click, std::move(click)));
}

void Button::SetTexture(ButtonStateOld btnState, std::unique_ptr<Texture> tex)
{
    if (btnState == ButtonStateOld::BT_Normal)
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

void Button::Update(const SimpleMath::Vector2& resolution, const Mouse::State& state)
{
    SimpleMath::Vector2 pos{ resolution * m_screenPos };

    int x = state.x - static_cast<int>(pos.x);
    int y = state.y - static_cast<int>(pos.y);

    m_state = ButtonStateOld::BT_Normal;

    if ((-m_origin.x < x && x < m_origin.x) && (-m_origin.y < y && y < m_origin.y))
        m_state = ButtonStateOld::BT_Over;

    if (m_state == ButtonStateOld::BT_Over && state.leftButton)
        m_state = ButtonStateOld::BT_Click;
}

void Button::OnDeviceLost()
{
    Reset();
}

void Button::LoadResources(ID3D12Device* device,
    DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload)
{
    std::vector<std::tuple<int, std::wstring>> filenames{
        {0, m_resPath + std::wstring(L"1.png")},
        {1, m_resPath + std::wstring(L"2.png")},
        {2, m_resPath + std::wstring(L"3.png")} };
    SetButtonTexture(device, descHeap, resUpload, filenames, this);
}

void Button::Render(DX12::SpriteBatch* sprite, const SimpleMath::Vector2& outputSize)
{
    m_textures[m_state]->Draw(sprite, outputSize * m_screenPos);
}

///////////////////////////////////////////////////////////////////////////////////
void SetButtonTexture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, ResourceUploadBatch& resUpload,
    std::vector<std::tuple<int, std::wstring>>& filenames, Button* button)
{
    std::ranges::for_each(filenames, [device, descHeap, &resUpload, bs{ ButtonStateOld::BT_Normal }, button](auto& file) mutable {
        std::unique_ptr<Texture> tex = std::make_unique<Texture>(device, descHeap);
        tex->Upload(resUpload, std::get<0>(file), std::get<1>(file));

        button->SetTexture(bs, std::move(tex));
        bs = static_cast<ButtonStateOld>(static_cast<int>(bs) + 1);
        });
}