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

	bool Setup(const UILayout& layout, const string& bindKey);

protected:
	PatchTextureStd9(const PatchTextureStd9& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
};

unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const UILayout& layout, const string& bindKey);
inline unique_ptr<PatchTextureStd9> CreatePatchTextureStd9(const string& bindKey) {
	return CreatePatchTextureStd9({}, bindKey);
}