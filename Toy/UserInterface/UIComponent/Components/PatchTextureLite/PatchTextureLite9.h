#pragma once
#include "PatchTextureLite.h"

class PatchTextureLite3;
class PatchTextureLite9 : public PatchTextureLite
{
public:
	~PatchTextureLite9();
	PatchTextureLite9() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTextureLite
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override;
	virtual bool FitToTextureSource() noexcept override;

protected:
	PatchTextureLite9(const PatchTextureLite9& other) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	bool ForEachPatchTextureLite3(predicate<PatchTextureLite3*, size_t> auto&& each);
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;
};
