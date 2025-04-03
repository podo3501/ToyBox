#include "pch.h"
#include "TexResCommandList.h"
#include "TexResCommandRegistry.h"
#include "../../../Utility.h"

bool TexResCommandList::AddFontKey(TextureResourceBinder* rb, const wstring& key, const TextureFontInfo& info) 
{
	return ApplyCommand<AddFontKeyCommand>(rb, key, info);
}

bool TexResCommandList::RemoveFontKey(TextureResourceBinder* rb, const wstring& key)
{
	return ApplyCommand<RemoveFontKeyCommand>(rb, key);
}

bool TexResCommandList::AddTextureKey(TextureResourceBinder* rb, const string& key, const TextureSourceInfo& info)
{
	return ApplyCommand<AddTextureKeyCommand>(rb, key, info);
}

bool TexResCommandList::RemoveTextureKey(TextureResourceBinder* rb, const string& key)
{
	return ApplyCommand<RemoveTextureKeyCommand>(rb, key);
}

bool TexResCommandList::RemoveKeyByFilename(TextureResourceBinder* rb, const wstring& filename)
{
	return ApplyCommand<RemoveKeyByFilenameCommand>(rb, filename);
}

bool TexResCommandList::RenameFontKey(TextureResourceBinder* rb, const wstring& preKey, const wstring& newKey)
{
	return ApplyCommand<RenameFontKeyCommand>(rb, preKey, newKey);
}

bool TexResCommandList::RenameTextureKey(TextureResourceBinder* rb, const string& preKey, const string& newKey)
{
	return ApplyCommand<RenameTextureKeyCommand>(rb, preKey, newKey);
}

bool TexResCommandList::ModifyTextureSourceInfo(TextureResourceBinder* rb, const string& key, const TextureSourceInfo& info)
{
	return ApplyCommand<ModifyTexSrcInfoCommand>(rb, key, info);
}