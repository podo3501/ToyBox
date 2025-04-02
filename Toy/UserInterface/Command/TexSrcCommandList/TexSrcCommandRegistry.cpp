#include "pch.h"
#include "TexSrcCommandRegistry.h"
#include "../Include/IRenderer.h"
#include "../../../Utility.h"
#include "../../TextureSourceBinder/TextureSourceBinder.h"

AddFontKeyCommand::AddFontKeyCommand(TextureSourceBinder* sb, const wstring& key, const TextureFontInfo& info) noexcept :
	TexSrcCommand{ sb },
	m_key{ key }, 
	m_record{ info }
{}

bool AddFontKeyCommand::Execute()
{
	m_record.previous = {};
	GetTarget()->AddFontKey(m_key, m_record.current);
	return true;
}

bool AddFontKeyCommand::Undo() { GetTarget()->RemoveFontKey(m_key); return true; }
bool AddFontKeyCommand::Redo() { return GetTarget()->AddFontKey(m_key, m_record.current); }