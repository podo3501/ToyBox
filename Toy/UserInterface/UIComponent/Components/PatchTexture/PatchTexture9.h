#pragma once
#include "PatchTexture.h"

struct SourceDivider;
struct TextureSourceInfo;
class PatchTexture3;

class PatchTexture9 : public PatchTexture
{
public:
	~PatchTexture9();
	PatchTexture9();

	static ComponentID GetTypeStatic() { return ComponentID::PatchTexture9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept override;

	bool Setup(const UILayout& layout, const string& bindKey);
	void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept;
	bool FitToTextureSource() noexcept override;
	const string& GetBindKey() const noexcept;

protected:
	PatchTexture9(const PatchTexture9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	bool ForEachPatchTexture3(predicate<PatchTexture3*, size_t> auto&& each);
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;
};

unique_ptr<PatchTexture9> CreatePatchTexture9(const UILayout& layout, const string& bindKey);