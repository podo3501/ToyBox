#pragma once
#include "Stubs/Include/IRendererStub.h"

class MockTextureLoad : public ITextureLoadStub
{
public:
	virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset) override;
	virtual bool LoadFont(const wstring& filename, size_t& outIndex) override;
};

class MockTextureController : public ITextureControllerStub
{
public:
	virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override;
};

class MockRenderer final : public IRendererStub
{
public:
	~MockRenderer();
	MockRenderer();

	virtual bool LoadTextureBinder(ITextureBinder* textureBinder) override;
	virtual ITextureController* GetTextureController() const noexcept override;

private:
	unique_ptr<MockTextureLoad> m_mockTextureLoad;
	unique_ptr<MockTextureController> m_mockTextureController;
};