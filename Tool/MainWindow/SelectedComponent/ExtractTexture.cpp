#include "pch.h"
#include "ExtractTexture.h"
#include "../Include/IRenderer.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UIComponentEx.h"

bool ExtractTexture::LoadResources(ILoadData* load)
{
    XMUINT2 size{};
    auto result = load->LoadTexture(GetResourceFullFilename(m_filename), nullptr, m_index, &size);
    SetSize(size);
    return result;
}

void ExtractTexture::ImplementRender(IRender* render) const
{
    const auto& size = GetSize();
    RECT source(0, 0, size.x, size.y);
    RECT destination(source);

    render->Render(m_index, destination, &source);
}