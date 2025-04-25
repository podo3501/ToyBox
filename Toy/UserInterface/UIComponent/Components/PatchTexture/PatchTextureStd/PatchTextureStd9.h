#pragma once
#include "PatchTextureStd.h"

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

	bool Setup(const UILayout& layout, const string& bindKey);
	void ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept;

protected:
	PatchTextureStd9(const PatchTextureStd9& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
};

unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const UILayout& layout, const string& bindKey);
inline unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const string& bindKey) {
	return CreatePatchTextureStd9({}, bindKey);
}