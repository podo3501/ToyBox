#pragma once
#include "Stubs/Include/IRendererStub.h"

class MockTextureLoad : public ITextureLoadStub
{
public:
	virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset) override;
	virtual bool LoadFont(const wstring& filename, size_t& outIndex) override;
};

class MockRenderer final : public IRendererStub
{
public:
	~MockRenderer();
	MockRenderer();

	virtual bool LoadTextureBinder(ITextureBinder* textureBinder) override;

private:
	unique_ptr<MockTextureLoad> m_mockTextureLoad;
};