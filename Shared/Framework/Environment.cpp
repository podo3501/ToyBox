#include "pch.h"
#include "Environment.h"

Environment::Environment(const wstring& resourcePath, const Vector2& resolution) :
	m_fontPath{ L"UI/Font/" },
	m_resourcePath{ resourcePath },
	m_resolution{ resolution }
{}

Rectangle Environment::GetRectResolution() const noexcept
{
	Rectangle rect{ 0, 0, static_cast<long>(m_resolution.x), static_cast<long>(m_resolution.y) };
	return rect;
}

wstring Environment::GetResourceFullFilename(const wstring& filename) const noexcept
{
	if (filesystem::path(filename).is_absolute())
		return filename;

	if (filename.find(m_resourcePath) == std::wstring::npos)
		return m_resourcePath + filename;

	return filename;
}

static void NormalizePath(wstring& path, char targetSeparator = '/') 
{
	char currentSeparator = (targetSeparator == '/') ? '\\' : '/';
	std::replace(path.begin(), path.end(), currentSeparator, targetSeparator);
}

wstring Environment::GetRelativePath(const wstring& fullPath) const noexcept
{
	if (fullPath.empty()) return {};

	filesystem::path full = filesystem::absolute(fullPath);
	filesystem::path base = filesystem::absolute(m_resourcePath);

	wstring relativePath = fullPath;
	if (full.wstring().find(base.wstring()) != wstring::npos)
		relativePath = full.wstring().substr(base.wstring().length());

	NormalizePath(relativePath);
	return relativePath;
}
