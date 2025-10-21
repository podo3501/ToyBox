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
	//Text°ü·Ã
	virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override;

	//Texture
	virtual bool CreateRenderTexture(IComponent* component, const Rectangle& targetRect, size_t& outIndex, UINT64* outGfxMemOffset) override;
};

class MockTextureRender : public ITextureRenderStub
{
public:
	virtual ~MockTextureRender() = default;

	MOCK_METHOD(void, Render, (size_t index, const RECT& dest, const RECT* source), (override));
	MOCK_METHOD(void, DrawString, 
		(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color), (const override));
};

class MockRenderer final : public IRendererStub
{
public:
	~MockRenderer();
	MockRenderer();

	MOCK_METHOD(void, AddRenderComponent, (IComponent* component), (override));
	virtual bool LoadTextureBinder(ITextureBinder* textureBinder) override;
	virtual ITextureController* GetTextureController() const noexcept override;

private:
	unique_ptr<MockTextureLoad> m_mockTextureLoad;
	unique_ptr<MockTextureController> m_mockTextureController;
};