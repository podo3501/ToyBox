#include "pch.h"
#include "TextureLoadBinder.h"
#include "../../Utility.h"

bool TextureLoadBinder::LoadResources(ITextureLoad* load)
{
	for (auto& texture : m_textures)
	{
		auto curIndex = texture.GetIndex();
		if (curIndex) continue; // 이 클래스는 자주 로딩 할 수 있기 때문에 한번 로딩 했으면 두번 하지 않는다.

		ReturnIfFalse(texture.LoadResource(load));
	}

	return true;
}

auto TextureLoadBinder::FindTextureByFilename(const wstring& filename) const noexcept
{
	return ranges::find_if(m_textures, [&filename](auto& texInfo) {
		return texInfo.filename == filename;
		});
}

void TextureLoadBinder::AddTexture(const wstring& filename) noexcept
{
	auto it = FindTextureByFilename(filename);
	if (it != m_textures.end()) return;

	m_textures.emplace_back(TextureSourceInfo{ filename });
}

optionalRef<TextureSourceInfo> TextureLoadBinder::GetSourceInfo(const wstring& filename) const noexcept
{
	auto it = FindTextureByFilename(filename);
	if (it == m_textures.end()) return nullopt;

	return cref(*it);
}
