#pragma once
#include "PatchTextureStd.h"
#include "../PatchTextureImpl.h"

struct TextureSourceInfo;
class PatchTextureStd3;

class PatchTextureStd9 : public PatchTextureStd
{
public:
	~PatchTextureStd9();
	PatchTextureStd9() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureStd9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTexture
	virtual bool ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept override;
	virtual const string& GetBindKey() const noexcept override;

	bool Setup(const UILayout& layout, const string& bindKey);

protected:
	PatchTextureStd9(const PatchTextureStd9& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual vector<Rectangle> GetChildSourceList() const noexcept override;
};

unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const UILayout& layout, const string& bindKey);