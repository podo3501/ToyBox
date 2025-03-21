#pragma once

struct ITextureLoad;
class TextureInfo
{
public:
	virtual ~TextureInfo() {};
	TextureInfo() noexcept;
	inline size_t GetIndex() const noexcept { return *m_index; }

protected:
	inline void SetIndex(size_t index) noexcept { m_index = index; }
	inline void SetTextureLoader(ITextureLoad* load) noexcept { m_texLoader = load; }
	void Release() noexcept;

private:
	ITextureLoad* m_texLoader;
	optional<size_t> m_index;
};