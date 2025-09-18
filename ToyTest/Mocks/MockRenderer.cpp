#include "pch.h"
#include "MockRenderer.h"
#include "../Include/ITextureBinder.h"

struct MockTextureInfo
{
	size_t index;
	XMUINT2 size;
};

unordered_map<wstring, MockTextureInfo> g_textureInfos
{
	{L"Resources/UI/Font/CourierNewBoldS18.spritefont", { 0, {} } },
	{L"Resources/UI/Font/MaleunGothicS16.spritefont", { 1, {} } },
	{L"Resources/UI/SampleTexture/Sample_0.png", { 2, { 512, 512 } } },
	{L"Resources/UI/SampleTexture/Option.png", { 3, { 512, 512 } } },
};

bool MockTextureLoad::LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset)
{
	auto it = g_textureInfos.find(filename);
	if (it == g_textureInfos.end())
		return false;

	outIndex = it->second.index;
	if (outSize)
		*outSize = it->second.size;

	return true;
}

bool MockTextureLoad::LoadFont(const wstring& filename, size_t& outIndex)
{
	auto it = g_textureInfos.find(filename);
	if (it == g_textureInfos.end())
		return false;

	outIndex = it->second.index;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////

MockRenderer::~MockRenderer() {}
MockRenderer::MockRenderer() :
	m_mockTextureLoad(make_unique<MockTextureLoad>())
{
}

bool MockRenderer::LoadTextureBinder(ITextureBinder* textureBinder)
{
	return textureBinder->LoadResources(m_mockTextureLoad.get());
}