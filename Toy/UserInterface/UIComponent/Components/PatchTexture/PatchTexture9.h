#pragma once
#include "PatchTexture.h"
#include "PatchTextureImpl.h"

struct TextureSourceInfo;
class PatchTexture3;

class PatchTexture9 : public PatchTexture
{
public:
	~PatchTexture9();
	PatchTexture9() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTexture9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTexture
	virtual bool FitToTextureSource() noexcept override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept override;
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept override;
	virtual const string& GetBindKey() const noexcept override;

	bool Setup(const UILayout& layout, const string& bindKey);

protected:
	PatchTexture9(const PatchTexture9& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	PatchTextureImpl m_impl;
};

unique_ptr<PatchTexture9> CreatePatchTexture9(const UILayout& layout, const string& bindKey);