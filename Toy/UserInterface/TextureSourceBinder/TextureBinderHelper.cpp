#include "pch.h"
#include "TextureBinderHelper.h"
#include "TextureSourceBinder.h"
#include "../UIComponent/UIUtility.h"
#include "../UIComponent/UIType.h"
#include "../../Utility.h"

optionalRef<vector<Rectangle>> GetRectangles(TextureSourceBinder* sb, const string& key) noexcept
{
    if (auto infoRef = sb->GetTextureSourceInfo(key); infoRef) return cref(infoRef->get().sources);
    return nullopt;
}

Rectangle GetTextureArea(TextureSourceBinder* sb, const string& key, int index) noexcept
{
    if (auto infoRef = sb->GetTextureSourceInfo(key); infoRef) return infoRef->get().GetSource(index);
    return {};
}

SourceDivider GetSourceDivider(TextureSourceBinder* sb, const string& key) noexcept
{
    auto infoRef = sb->GetTextureSourceInfo(key);
    if (!infoRef) return {};
    
    return GetSourceDivider(infoRef->get());
}

SourceDivider GetSourceDivider(const TextureSourceInfo& sourceInfo) noexcept
{
    ImagePart imgPart = sourceInfo.imagePart;
    const vector<Rectangle>& sources = sourceInfo.sources;
    Rectangle mergedSource = CombineRectangles(sources);
    
    SourceDivider srcDivider{ mergedSource };
    if (imgPart == ImagePart::One) return srcDivider;

    const Rectangle& leftSource = sources[0];
    const int leftX = leftSource.width;
    const int rightX = sources[2].x - mergedSource.x;
    const int top = leftSource.height;
    const int bottom = sources[(sources.size() == 3) ? 2 : 6].y - mergedSource.y;

    switch (imgPart)
    {
    case ImagePart::ThreeH: srcDivider.list = { leftX, rightX }; break;
    case ImagePart::ThreeV: srcDivider.list = { top, bottom }; break;
    case ImagePart::Nine: srcDivider.list = { leftX, rightX, top, bottom }; break;
    }

    return srcDivider;
}

pair<wstring, TextureFontInfo> GetTextureFontInfo(TextureSourceBinder* sb, const wstring& filename) noexcept
{
    const auto& key = sb->GetFontKey(filename);
    if (key.empty()) return {};

    const auto& infoRef = sb->GetTextureFontInfo(key);
    return pair{ key, infoRef->get() };
}

vector<pair<string, TextureSourceInfo>> GetTextureSourceInfo(TextureSourceBinder* sb, const wstring& filename) noexcept
{
    vector<pair<string, TextureSourceInfo>> result;
    
    vector<string> keys = sb->GetTextureKeys(filename);
    if (keys.empty()) return result;

    ranges::transform(keys, back_inserter(result), [sb](const auto& key) {
        const auto& infoRef = sb->GetTextureSourceInfo(key);
        return pair{ key, infoRef->get() };
        });

    return result;
}

vector<Rectangle> GetSources(ImagePart imgPart, const SourceDivider& sourceDivider) noexcept
{
    if (imgPart == ImagePart::One) return { sourceDivider.rect };

    vector<int> widths{}, heights{};
    bool success = false;
    switch (imgPart)
    {
    case ImagePart::ThreeH: success = GetSizeDividedByThree(DirectionType::Horizontal, sourceDivider, widths, heights); break;
    case ImagePart::ThreeV: success = GetSizeDividedByThree(DirectionType::Vertical, sourceDivider, widths, heights); break;
    case ImagePart::Nine: success = GetSizeDividedByNine(sourceDivider, widths, heights); break;
    default: return {};
    }

    return success ? GetSourcesFromArea(sourceDivider.rect, widths, heights) : vector<Rectangle>{};
}