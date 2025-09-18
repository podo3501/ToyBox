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

Rectangle MockTextureController::MeasureText(size_t index, const wstring& text, const Vector2& position)
{
	//index�� � ��Ʈ���� Ȯ���ϴ� �ǵ� �̰� ����.
	const long fixedWidthPerChar = 10;  // ���ڴ� ���� ��
	const long fixedHeight = 20;        // ���� ����
	long width = static_cast<long>(text.size()) * fixedWidthPerChar;

	return Rectangle{
		static_cast<long>(position.x),
		static_cast<long>(position.y),
		width,
		fixedHeight
	};
}

/////////////////////////////////////////////////////////////////////////////////

MockRenderer::~MockRenderer() {}
MockRenderer::MockRenderer() :
	m_mockTextureLoad(make_unique<MockTextureLoad>()),
	m_mockTextureController(make_unique<MockTextureController>())
{
}

bool MockRenderer::LoadTextureBinder(ITextureBinder* textureBinder)
{
	return textureBinder->LoadResources(m_mockTextureLoad.get());
}

ITextureController* MockRenderer::GetTextureController() const noexcept
{
	return m_mockTextureController.get();
}