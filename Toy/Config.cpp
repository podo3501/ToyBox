#include "pch.h"
#include "Config.h"

static wstring g_resourcePath{};
static Vector2 g_resolution{};

void InitializeConfig(const wstring& resourcePath, const Vector2& resolution) noexcept
{
	g_resourcePath = resourcePath;
	g_resolution = resolution;
}

const wstring& GetResourcePath() noexcept { return g_resourcePath; }
const Vector2& GetResolution() noexcept { return g_resolution; }