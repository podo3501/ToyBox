#include "pch.h"
#include "TextureSourceInfo.h"
#include "../Include/IRenderer.h"
#include "../JsonHelper.h"
#include "../../Config.h"
#include "../../Utility.h"
#include "../UIType.h"

TextureSourceInfo::~TextureSourceInfo()
{
    Release();
}

TextureSourceInfo::TextureSourceInfo() noexcept :
    imagePart{ ImagePart::One },
    m_texLoader{ nullptr }
{
}

TextureSourceInfo::TextureSourceInfo(const TextureSourceInfo& other) noexcept :
    filename{ other.filename },
    imagePart{ other.imagePart },
    sources{ other.sources },
    m_texLoader{ nullptr }
{
}

TextureSourceInfo::TextureSourceInfo(const wstring& _filename, ImagePart _imagePart, const vector<Rectangle>& _sources) noexcept :
    filename{ _filename },
    imagePart{ _imagePart },
    sources{ _sources },
    m_texLoader{ nullptr }
{
}

bool TextureSourceInfo::operator==(const TextureSourceInfo& o) const noexcept
{
    return (tie(filename, sources) == tie(o.filename, o.sources));
}

void TextureSourceInfo::Release() noexcept
{
    if (m_texLoader && m_index)
    {
        m_texLoader->ReleaseTexture(*m_index);
        m_texLoader = nullptr;
        m_index = nullopt;
    }
}

bool TextureSourceInfo::LoadResource(ITextureLoad* load)
{
    if (filename.empty()) return false;
    Release();

    size_t index{ 0 };
    ReturnIfFalse(load->LoadTexture(GetResourceFullFilename(filename), index, nullptr, nullptr));
    m_index = index;
    m_texLoader = load;

    return true;
}

void TextureSourceInfo::SerializeIO(JsonOperation& operation)
{
    operation.Process("Filename", filename);
    operation.Process("ImagePart", imagePart);
    operation.Process("Sources", sources);
}