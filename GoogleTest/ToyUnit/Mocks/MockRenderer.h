#pragma once
#include "Stubs/Include/IRendererStub.h"

struct MockTextureInfo
{
	size_t index;
	XMUINT2 size;
};

class MockTextureTable
{
public:
	~MockTextureTable() {};
	MockTextureTable() {};
	bool AddTextureInfo(const wstring& filename, size_t index, const XMUINT2& size) noexcept;
	optionalRef<MockTextureInfo> GetTextureInfo(const wstring& filename) const noexcept;
	size_t GetSize() const noexcept;

private:
	unordered_map<wstring, MockTextureInfo> m_textureInfos;
};

class MockTextureLoad : public ITextureLoadStub
{
public:
	MockTextureLoad(MockTextureTable* texTable);

	virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset) override;
	virtual bool LoadFont(const wstring& filename, size_t& outIndex) override;

private:
	MockTextureTable* m_texTable{ nullptr };
};

class MockTextureController : public ITextureControllerStub
{
public:
	MockTextureController(MockTextureTable* texTable);
	//Text관련
	virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override;
	virtual float GetLineSpacing(size_t index) const noexcept override;

	//Texture
	virtual bool CreateRenderTexture(IComponent* component, const Rectangle& targetRect, size_t& outIndex, UINT64* outGfxMemOffset) override;

private:
	MockTextureTable* m_texTable{ nullptr };
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
	bool RegisterMockTextureInfo(const wstring& filename, size_t index, const XMUINT2& size) noexcept;
	bool RegisterMockTextureInfos(const vector<tuple<wstring, size_t, XMUINT2>>& texInfos) noexcept;

private:
	unique_ptr<MockTextureTable> m_mockTextureTable; //가짜 텍스쳐 정보를 담고 있는 클래스
	unique_ptr<MockTextureLoad> m_mockTextureLoad;
	unique_ptr<MockTextureController> m_mockTextureController;
};