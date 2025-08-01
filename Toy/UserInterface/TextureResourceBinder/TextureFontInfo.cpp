#include "pch.h"
#include "TextureFontInfo.h"
#include "../Include/IRenderer.h"
#include "../JsonOperation/JsonOperation.h"
#include "Config.h"
#include "Utils/CommonUtil.h"

TextureFontInfo::~TextureFontInfo() = default;
TextureFontInfo::TextureFontInfo() noexcept = default;
TextureFontInfo::TextureFontInfo(const TextureFontInfo& other) noexcept :
    filename{ other.filename }
{}

TextureFontInfo::TextureFontInfo(const wstring& _filename) noexcept :
    filename{ _filename }
{}

bool TextureFontInfo::operator==(const TextureFontInfo& o) const noexcept
{
    return (filename == o.filename);
}

bool TextureFontInfo::LoadResource(ITextureLoad* load)
{
    if (filename.empty()) return false;
    Release();

    size_t index{ 0 };
    ReturnIfFalse(load->LoadFont(GetResourceFullFilename(filename), index));
    SetIndex(index);
    SetTextureLoader(load);

    return true;
}

void TextureFontInfo::SerializeIO(JsonOperation& operation)
{
    operation.Process("Filename", filename);
}