#include "pch.h"
#include "Config.h"

static wstring g_resourcePath{};
void SetResourcePath(const wstring& resourcePath) noexcept
{
	g_resourcePath = resourcePath;
}

const wstring& GetResourcePath() noexcept
{
	return g_resourcePath;
}