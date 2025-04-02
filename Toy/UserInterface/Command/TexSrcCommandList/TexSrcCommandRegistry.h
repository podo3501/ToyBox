#pragma once
#include "../Command.h"
#include "../CommandType.h"
#include "../../TextureSourceBinder/TextureFontInfo.h"

struct TextureFontInfo;
class TextureSourceBinder;

class AddFontKeyCommand : public TexSrcCommand
{
public:
	AddFontKeyCommand(TextureSourceBinder* srcBinder, const wstring& bindingKey, const TextureFontInfo& fontInfo) noexcept;

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