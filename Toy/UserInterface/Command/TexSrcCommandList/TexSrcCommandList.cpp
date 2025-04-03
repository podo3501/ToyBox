#include "pch.h"
#include "TexSrcCommandList.h"
#include "TexSrcCommandRegistry.h"
#include "../../../Utility.h"

bool TexSrcCommandList::AddFontKey(TextureSourceBinder * sb, const wstring& key, const TextureFontInfo& info) 
{
	return ApplyCommand<AddFontKeyCommand>(sb, key, info);
}

bool TexSrcCommandList::RemoveFontKey(TextureSourceBinder* sb, const wstring& key)
{
	return ApplyCommand<RemoveFontKeyCommand>(sb, key);
}

bool TexSrcCommandList::AddTextureKey(TextureSourceBinder* sb, const string& key, const TextureSourceInfo& info)
{
	return ApplyCommand<AddTextureKeyCommand>(sb, key, info);
}

bool TexSrcCommandList::RemoveTextureKey(TextureSourceBinder* sb, const string& key)
{
	return ApplyCommand<RemoveTextureKeyCommand>(sb, key);
}

bool TexSrcCommandList::RemoveKeyByFilename(TextureSourceBinder* sb, const wstring& filename)
{
	return ApplyCommand<RemoveKeyByFilenameCommand>(sb, filename);
}

bool TexSrcCommandList::RenameFontKey(TextureSourceBinder* sb, const wstring& preKey, const wstring& newKey)
{
	return ApplyCommand<RenameFontKeyCommand>(sb, preKey, newKey);
}

bool TexSrcCommandList::RenameTextureKey(TextureSourceBinder* sb, const string& preKey, const string& newKey)
{
	return ApplyCommand<RenameTextureKeyCommand>(sb, preKey, newKey);
}

bool TexSrcCommandList::ModifyTextureSourceInfo(TextureSourceBinder* sb, const string& key, const TextureSourceInfo& info)
{
	return ApplyCommand<ModifyTexSrcInfoCommand>(sb, key, info);
}