#include "pch.h"
#include "MockRenderer.h"
#include "ITextureBinder.h"

bool MockTextureTable::AddTextureInfo(const wstring& filename, size_t index, const XMUINT2& size) noexcept
{
	if (m_textureInfos.find(filename) != m_textureInfos.end())
		return false;

	MockTextureInfo info{ index, size };
	m_textureInfos.insert({ filename, info });
	return true;
}

optionalRef<MockTextureInfo> MockTextureTable::GetTextureInfo(const wstring& filename) const noexcept
{
	auto it = m_textureInfos.find(filename);
	if (it == m_textureInfos.end()) return nullopt;

	return cref(it->second);
}

size_t MockTextureTable::GetSize() const noexcept
{
	return m_textureInfos.size();
}

/////////////////////////////////////////////////////////////////////////////////

MockTextureLoad::MockTextureLoad(MockTextureTable* texTable) :
	m_texTable{ texTable }
{}

bool MockTextureLoad::LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset)
{
	auto texInfo = m_texTable->GetTextureInfo(filename);
	ReturnIfFalse(texInfo);
	
	outIndex = texInfo->get().index;
	if (outSize) *outSize = texInfo->get().size;

	return true;
}

bool MockTextureLoad::LoadFont(const wstring& filename, size_t& outIndex)
{
	auto texInfo = m_texTable->GetTextureInfo(filename);
	ReturnIfFalse(texInfo);

	outIndex = texInfo->get().index;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////

MockTextureController::MockTextureController(MockTextureTable* texTable) :
	m_texTable{ texTable }
{}

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

//���� �ؽ��ĸ� ������ٰ� �����ϰ� ��¥ �����ؽ��� �ε����� �������ش�.
bool MockTextureController::CreateRenderTexture(IComponent* component, const Rectangle& targetRect, size_t& outIndex, UINT64* outGfxMemOffset)
{
	size_t index = m_texTable->GetSize();
	wstring key = L"RenderTexture_" + to_wstring(index);

	ReturnIfFalse(m_texTable->AddTextureInfo(key, index, {} ));

	outIndex = index;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////

MockRenderer::~MockRenderer() {}
MockRenderer::MockRenderer() :
	m_mockTextureTable(make_unique<MockTextureTable>()),
	m_mockTextureLoad(make_unique<MockTextureLoad>(m_mockTextureTable.get())),
	m_mockTextureController(make_unique<MockTextureController>(m_mockTextureTable.get()))
{}

bool MockRenderer::RegisterMockTextureInfo(const wstring& filename, size_t index, const XMUINT2& size) noexcept
{
	return m_mockTextureTable->AddTextureInfo(filename, index, size);
}

bool MockRenderer::RegisterMockTextureInfos(const vector<tuple<wstring, size_t, XMUINT2>>& texInfos) noexcept
{
	for (const auto& [filename, index, size] : texInfos)
		ReturnIfFalse(RegisterMockTextureInfo(filename, index, size));

	return true;
}

bool MockRenderer::LoadTextureBinder(ITextureBinder* textureBinder)
{
	return textureBinder->LoadResources(m_mockTextureLoad.get());
}

ITextureController* MockRenderer::GetTextureController() const noexcept
{
	return m_mockTextureController.get();
}