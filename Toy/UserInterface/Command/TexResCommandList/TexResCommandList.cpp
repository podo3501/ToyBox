#include "pch.h"
#include "TexResCommandList.h"
#include "TexResCommandRegistry.h"
#include "Utils/CommonUtil.h"

TexResCommandList::~TexResCommandList() = default;
TexResCommandList::TexResCommandList(TextureResourceBinder* resBinder) noexcept :
	m_resBinder{ resBinder }
{}

bool TexResCommandList::AddFontKey(const wstring& key, const TextureFontInfo& info) 
{ return ApplyCommand<AddFontKeyCommand>(m_resBinder, key, info); }

bool TexResCommandList::RemoveFontKey(const wstring& key)
{ return ApplyCommand<RemoveFontKeyCommand>(m_resBinder, key); }

bool TexResCommandList::AddTextureKey(const string& key, const TextureSourceInfo& info)
{ return ApplyCommand<AddTextureKeyCommand>(m_resBinder, key, info); }

bool TexResCommandList::RemoveTextureKey(const string& key)
{ return ApplyCommand<RemoveTextureKeyCommand>(m_resBinder, key); }

bool TexResCommandList::RemoveKeyByFilename(const wstring& filename)
{ return ApplyCommand<RemoveKeyByFilenameCommand>(m_resBinder, filename); }

bool TexResCommandList::RenameFontKey(const wstring& preKey, const wstring& newKey)
{ return ApplyCommand<RenameFontKeyCommand>(m_resBinder, preKey, newKey); }

bool TexResCommandList::RenameTextureKey(const string& preKey, const string& newKey)
{ return ApplyCommand<RenameTextureKeyCommand>(m_resBinder, preKey, newKey); }

bool TexResCommandList::ModifyTextureSourceInfo(const string& key, const TextureSourceInfo& info)
{ return ApplyCommand<ModifyTexSrcInfoCommand>(m_resBinder, key, info); }