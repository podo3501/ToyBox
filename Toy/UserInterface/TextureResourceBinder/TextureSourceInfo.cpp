#include "pch.h"
#include "TextureSourceInfo.h"
#include "../Include/IRenderer.h"
#include "../JsonOperation/JsonHelper.h"
#include "Config.h"
#include "Utility.h"
#include "../UIComponent/UIType.h"

TextureSourceInfo::~TextureSourceInfo() = default;
TextureSourceInfo::TextureSourceInfo() noexcept :
    imagePart{ ImagePart::One }
{}

TextureSourceInfo::TextureSourceInfo(const wstring& _filename, ImagePart _imagePart, const vector<Rectangle>& _sources) noexcept :
    filename{ _filename },
    imagePart{ _imagePart },
    sources{ _sources }
{}

TextureSourceInfo::TextureSourceInfo(const wstring& _filename) noexcept :
    filename{ _filename },
    imagePart{ ImagePart::One },
    sources{}
{}

bool TextureSourceInfo::operator==(const TextureSourceInfo& o) const noexcept
{
    return (tie(filename, imagePart, sources) == tie(o.filename, o.imagePart, o.sources));
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

    if (imagePart == ImagePart::One && sources.empty())
        sources.emplace_back(Rectangle{ 0, 0, static_cast<long>(texSize.x), static_cast<long>(texSize.y) });

    return true;
}

void TextureSourceInfo::SerializeIO(JsonOperation& operation)
{
    operation.Process("Filename", filename);
    operation.Process("ImagePart", imagePart);
    operation.Process("Sources", sources);
}