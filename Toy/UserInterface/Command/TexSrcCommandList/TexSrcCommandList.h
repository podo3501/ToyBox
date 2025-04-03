#pragma once
#include "../Command.h"
#include "../CommandList.h"

struct TextureFontInfo;
struct TextureSourceInfo;
class TextureSourceBinder;
class TexSrcCommandList : public CommandList<TexSrcCommand>
{
public:
	~TexSrcCommandList() = default;
	TexSrcCommandList() = default;

	bool AddFontKey(TextureSourceBinder* sb, const wstring& bindingKey, const TextureFontInfo& fontInfo);
	bool RemoveFontKey(TextureSourceBinder* sb, const wstring& key);

	bool AddTextureKey(TextureSourceBinder* sb, const string& key, const TextureSourceInfo& info);
	bool RemoveTextureKey(TextureSourceBinder* sb, const string& key);

	bool RemoveKeyByFilename(TextureSourceBinder* sb, const wstring& filename);

	bool RenameFontKey(TextureSourceBinder* sb, const wstring& preKey, const wstring& newKey);
	bool RenameTextureKey(TextureSourceBinder* sb, const string& preKey, const string& newKey);

	bool ModifyTextureSourceInfo(TextureSourceBinder* sb, const string& key, const TextureSourceInfo& info);
};
