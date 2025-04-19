#pragma once
#include "../../UIComponent.h"

struct ITextureController;
struct TextureSourceInfo;

class PatchTextureBase : public UIComponent
{
public:
	~PatchTextureBase();
	virtual bool FitToTextureSource() noexcept = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	inline optional<TextureSlice> GetTextureSlice() const noexcept { return m_texSlice; }

protected:
	PatchTextureBase();
	PatchTextureBase(TextureSlice texSlice) noexcept;
	PatchTextureBase(const PatchTextureBase& other) noexcept;
	inline void SetTextureSlice(TextureSlice texSlice) noexcept { m_texSlice = texSlice; }

private:
	optional<TextureSlice> m_texSlice{ nullopt }; //?!? 이 변수는 저장하지 않았기 때문에 문제가 될 수 있다.

	friend class PatchTextureImpl;
};

class PatchTexture : public PatchTextureBase
{
public:
	~PatchTexture();
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept = 0;
	virtual const string& GetBindKey() const noexcept = 0;

protected:
	using PatchTextureBase::PatchTextureBase;
	PatchTexture();
	PatchTexture(const PatchTexture& other) noexcept;
};

unique_ptr<PatchTexture> CreatePatchTexture(const UILayout& layout, TextureSlice texSlice, const string& bindKey);