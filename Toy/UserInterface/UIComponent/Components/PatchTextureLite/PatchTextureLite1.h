#pragma once
#include "PatchTextureLite.h"
#include "../PatchTexture/PatchTextureCoord.h"

class PatchTextureLite1 : public PatchTextureLite
{
public:
	~PatchTextureLite1();
	PatchTextureLite1();

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTextureLite
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override { m_coord.SetIndexedSource(index, source); }
	virtual bool FitToTextureSource() noexcept override { return m_coord.FitToTextureSource(); }

	inline const Rectangle& GetSource() const noexcept { return m_coord.GetSource(); }

protected:
	PatchTextureLite1(const PatchTextureLite1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual void ImplementRender(ITextureRender* render) const override { m_coord.Render(render); }

private:
	PatchTextureCoord m_coord;
};