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
	virtual bool BindSourceInfo(size_t index, const vector<Rectangle>& sources) override;

	bool Setup(const XMUINT2& size);

protected:
	PatchTextureLite9(const PatchTextureLite9& other) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
};

unique_ptr<PatchTextureLite9> CreatePatchTextureLite9(const XMUINT2& size);