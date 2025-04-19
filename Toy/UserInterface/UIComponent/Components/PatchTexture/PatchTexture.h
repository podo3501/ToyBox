#pragma once
#include "../../UIComponent.h"

struct ITextureController;
struct TextureSourceInfo;

class PatchTexture : public UIComponent
{
public:
	~PatchTexture();
	virtual bool FitToTextureSource() noexcept = 0;
	virtual void SetIndexedSource(size_t, const vector<Rectangle>&) noexcept = 0;
	inline optional<TextureSlice> GetTextureSlice() const noexcept { return m_texSlice; }

protected:
	PatchTexture();
	PatchTexture(TextureSlice texSlice) noexcept;
	PatchTexture(const PatchTexture& other) noexcept;
	inline void SetTextureSlice(TextureSlice texSlice) noexcept { m_texSlice = texSlice; }

private:
	optional<TextureSlice> m_texSlice{ nullopt }; //?!? �� ������ �������� �ʾұ� ������ ������ �� �� �ִ�.

	friend class PatchTextureImpl;
};