#include "pch.h"
#include "ExtractTexture.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UIComponentEx.h"

bool ExtractTexture::LoadResources(ILoadData* load)
{
    XMUINT2 size{};
    auto result = load->LoadTexture(GetResourceFullFilename(m_filename), nullptr, m_index, &size);
    GetTransformContainer().SetSize(size);
    return result;
}

void ExtractTexture::ImplementRender(IRender* render) const
{
    const auto& size = UIComponentEx::GetSize(this);
    RECT source(0, 0, size.x, size.y);
    RECT destination(source);

    render->Render(m_index, destination, &source);
}