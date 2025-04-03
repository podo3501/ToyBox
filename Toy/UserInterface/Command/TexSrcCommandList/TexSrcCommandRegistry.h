#pragma once
#include "../Command.h"
#include "../CommandType.h"
#include "../../TextureSourceBinder/TextureFontInfo.h"
#include "../../TextureSourceBinder/TextureSourceInfo.h"

class TextureSourceBinder;

struct TextureFontInfo;
class AddFontKeyCommand : public TexSrcCommand
{
public:
	AddFontKeyCommand(TextureSourceBinder* sb, const wstring& key, const TextureFontInfo& info) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::AddFontKey; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	wstring m_key;
	CommandRecord<TextureFontInfo> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemoveFontKeyCommand : public TexSrcCommand
{
public:
	RemoveFontKeyCommand(TextureSourceBinder* sb, const wstring& key) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::RemoveFontKey; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	wstring m_key;
	CommandRecord<TextureFontInfo> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TextureSourceInfo;
class AddTextureKeyCommand : public TexSrcCommand
{
public:
	AddTextureKeyCommand(TextureSourceBinder* sb, const string& key, const TextureSourceInfo& info) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::AddTextureKey; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	string m_key;
	CommandRecord<TextureSourceInfo> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemoveTextureKeyCommand : public TexSrcCommand
{
public:
	RemoveTextureKeyCommand(TextureSourceBinder* sb, const string& key) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::RemoveTextureKey; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	string m_key;
	CommandRecord<TextureSourceInfo> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemoveKeyByFilenameCommand : public TexSrcCommand
{
public:
	RemoveKeyByFilenameCommand(TextureSourceBinder* sb, const wstring& filename) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::RemoveKeyByFilename; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	using FontInfo = pair<wstring, TextureFontInfo>;
	using TextureInfos = vector<pair<string, TextureSourceInfo>>;

	wstring m_filename;
	CommandRecord<pair<FontInfo, TextureInfos>> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RenameFontKeyCommand : public TexSrcCommand
{
public:
	RenameFontKeyCommand(TextureSourceBinder* sb, const wstring& preKey, const wstring& newKey) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::RenameFontKey; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	CommandRecord<pair<wstring, wstring>> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RenameTextureKeyCommand : public TexSrcCommand
{
public:
	RenameTextureKeyCommand(TextureSourceBinder* sb, const string& preKey, const string& newKey) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::RenameTextureKey; }
	virtual bool IsMerge(TexSrcCommand*) noexcept { return false; }

private:
	CommandRecord<pair<string, string>> m_record;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ModifyTexSrcInfoCommand : public TexSrcCommand
{
public:
	ModifyTexSrcInfoCommand(TextureSourceBinder* sb, const string& key, const TextureSourceInfo& sourceInfo) noexcept;

	virtual bool Execute() override;
	virtual bool Undo() override;
	virtual bool Redo() override;

protected:
	virtual TexSrcCommandID GetTypeID() const noexcept override { return TexSrcCommandID::ModifyTexSrcInfo; }
	virtual void PostMerge(unique_ptr<TexSrcCommand> other) noexcept override;

private:
	string m_key;
	CommandRecord<TextureSourceInfo> m_record;
};


