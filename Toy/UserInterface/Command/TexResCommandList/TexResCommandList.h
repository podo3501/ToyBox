#pragma once
#include "../Command.h"
#include "../CommandList.h"

struct TextureFontInfo;
struct TextureSourceInfo;
class TextureResourceBinder;
class TexResCommandList : public CommandList<TexResCommand>
{
public:
	~TexResCommandList() = default;
	TexResCommandList() = default;

	bool AddFontKey(TextureResourceBinder* rb, const wstring& bindingKey, const TextureFontInfo& fontInfo);
	bool RemoveFontKey(TextureResourceBinder* rb, const wstring& key);

	bool AddTextureKey(TextureResourceBinder* rb, const string& key, const TextureSourceInfo& info);
	bool RemoveTextureKey(TextureResourceBinder* rb, const string& key);

	bool RemoveKeyByFilename(TextureResourceBinder* rb, const wstring& filename);

	bool RenameFontKey(TextureResourceBinder* rb, const wstring& preKey, const wstring& newKey);
	bool RenameTextureKey(TextureResourceBinder* rb, const string& preKey, const string& newKey);

	bool ModifyTextureSourceInfo(TextureResourceBinder* rb, const string& key, const TextureSourceInfo& info);
};
