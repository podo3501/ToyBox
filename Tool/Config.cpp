#include "pch.h"
#include "Config.h"
#include "../Toy/UserInterface/JsonHelper.h"

using namespace Tool;

static constexpr const wchar_t* ResolutionFilename = L"Tool/Config.json";
ResolutionType Config::m_resolutionType = ResolutionType::R800x600;
Config* Config::m_Instance = nullptr;

Config::Config()
{
	m_Instance = this;
	ReadJsonFile(ResolutionFilename, *this);
}

Config::~Config()
{}

void Config::SetResolution(ResolutionType type) noexcept
{
	if (m_resolutionType == type)
		return;

	m_resolutionType = type;
	WriteJsonFile(*m_Instance, ResolutionFilename);
}

ResolutionType Config::GetResolution() noexcept
{
	return m_resolutionType;
}

XMUINT2 Config::GetResolutionInCoordinate() noexcept
{
	return XMUINT2{ 800, 600 };
}

void Config::SerializeIO(JsonOperation& jsonOp)
{
    jsonOp.Process("Resolution", m_resolutionType);
}