#include "pch.h"
#include "TextureLoadBinder.h"
#include "../../Utility.h"

bool TextureLoadBinder::LoadResources(ITextureLoad* load)
{
	for (auto& texture : m_textures)
	{
		auto curIndex = texture.GetIndex();
		if (curIndex) continue; // �� Ŭ������ ���� �ε� �� �� �ֱ� ������ �ѹ� �ε� ������ �ι� ���� �ʴ´�.

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
