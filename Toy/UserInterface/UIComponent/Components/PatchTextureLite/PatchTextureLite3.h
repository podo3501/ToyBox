#pragma once
#include "PatchTextureLite.h"

class PatchTextureLite1;
class PatchTextureLite3 : public PatchTextureLite
{
public:
	~PatchTextureLite3();
	PatchTextureLite3();
	explicit PatchTextureLite3(DirectionType dirType) noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTextureLite
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override;
	virtual bool FitToTextureSource() noexcept override;

protected:
	PatchTextureLite3(const PatchTextureLite3& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	bool ForEachTex(predicate<PatchTextureLite1*, size_t> auto&& each);
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;

	DirectionType m_dirType{ DirectionType::Horizontal };
};
