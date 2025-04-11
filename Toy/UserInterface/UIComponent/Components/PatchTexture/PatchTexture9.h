#pragma once
#include "PatchTexture.h"

struct SourceDivider;
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

	bool Setup(const UILayout& layout, const string& bindKey);
	const string& GetBindKey() const noexcept;

protected:
	PatchTexture9(const PatchTexture9& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	bool ForEachPatchTexture3(predicate<PatchTexture3*, size_t> auto&& each);
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;
};

unique_ptr<PatchTexture9> CreatePatchTexture9(const UILayout& layout, const string& bindKey);