#include "pch.h"
#include "TextureSourceInfo.h"
#include "../Include/IRenderer.h"
#include "../JsonOperation/JsonHelper.h"
#include "Config.h"
#include "Utility.h"
#include "../UIComponent/UIType.h"

TextureSourceInfo::~TextureSourceInfo() = default;
TextureSourceInfo::TextureSourceInfo() noexcept :
    texSlice{ TextureSlice::One }
{}

TextureSourceInfo::TextureSourceInfo(const wstring& _filename, TextureSlice _texSlice, const vector<Rectangle>& _sources) noexcept :
    filename{ _filename },
    texSlice{ _texSlice },
    sources{ _sources }
{}

TextureSourceInfo::TextureSourceInfo(const wstring& _filename) noexcept :
    filename{ _filename },
    texSlice{ TextureSlice::One },
    sources{}
{}

bool TextureSourceInfo::operator==(const TextureSourceInfo& o) const noexcept
{
    return (tie(filename, texSlice, sources) == tie(o.filename, o.texSlice, o.sources));
}

bool TextureSourceInfo::LoadResource(ITextureLoad* load)
{
    if (filename.empty()) return false;
    Release();

    size_t index{ 0 };
    XMUINT2 texSize{};
    UINT64 gfxOffset{ 0 };
    ReturnIfFalse(load->LoadTexture(GetResourceFullFilename(filename), index, &texSize, &gfxOffset));

    SetIndex(index);
    SetGfxOffset(gfxOffset);
    SetTextureLoader(load);

    if (texSlice == TextureSlice::One && sources.empty())
        sources.emplace_back(Rectangle{ 0, 0, static_cast<long>(texSize.x), static_cast<long>(texSize.y) });

    return true;
}

void TextureSourceInfo::SerializeIO(JsonOperation& operation)
{
    operation.Process("Filename", filename);
    operation.Process("TextureSlice", texSlice);
    operation.Process("Sources", sources);
}