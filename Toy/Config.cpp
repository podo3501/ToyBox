#include "pch.h"
#include "Config.h"
#include <filesystem>

static wstring g_resourcePath{};
static Vector2 g_resolution{};

void InitializeConfig(const wstring& resourcePath, const Vector2& resolution) noexcept
{
	g_resourcePath = resourcePath;
	g_resolution = resolution;
}

//const wstring& GetResourcePath() noexcept { return g_resourcePath; }
const Vector2& GetResolution() noexcept { return g_resolution; }
Rectangle GetRectResolution() noexcept 
{ 
	Rectangle rect{ 0, 0, static_cast<long>(g_resolution.x), static_cast<long>(g_resolution.y) };
	return rect;
}

wstring GetResourceFullFilename(const wstring& filename) noexcept
{
	if(filesystem::path(filename).is_absolute())
		return filename;

	return g_resourcePath + filename;
}