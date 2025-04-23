#pragma once
#include "PatchTextureLite.h"
#include "../PatchTextureImpl.h"

class PatchTextureLite3;
class PatchTextureLite9 : public PatchTextureLite
{
public:
	~PatchTextureLite9();
	PatchTextureLite9() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTextureLite
	//virtual bool FitToTextureSource() noexcept override;
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override;

protected:
	PatchTextureLite9(const PatchTextureLite9& other) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual vector<Rectangle> GetChildSourceList() const noexcept override;

private:
	PatchTextureImpl m_impl;
};
