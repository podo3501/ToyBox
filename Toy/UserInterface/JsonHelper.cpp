#include "pch.h"
#include "JsonHelper.h"

map<const UIComponent*, wstring> JsonFile::m_filenames{};

wstring JsonFile::GetJsonFilename(const UIComponent* data) noexcept
{
	auto it = m_filenames.find(data);
	if (it != m_filenames.end()) return it->second;

	return L"";
}

void JsonFile::SetJsonFilename(const UIComponent* data, const wstring& filename) noexcept
{
	m_filenames[data] = filename;
}