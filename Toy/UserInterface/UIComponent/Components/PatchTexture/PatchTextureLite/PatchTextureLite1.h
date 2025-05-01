#pragma once
#include "PatchTextureLite.h"
#include "../PatchTextureCoord.h"

class PatchTextureLite1 : public PatchTextureLite
{
public:
	~PatchTextureLite1();
	PatchTextureLite1();

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	//PatchTextureLite
	virtual bool FitToTextureSource() noexcept override { return m_coord.FitToTextureSource(); }
	virtual UIComponent* GetCenterComponent() noexcept override { return this; }
	virtual bool BindSourceInfo(size_t index, const vector<Rectangle>& sources) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override { m_coord.SetIndexedSource(index, source); }

	bool Setup(const XMUINT2& size = {});
	inline const Rectangle& GetSource() const noexcept { return m_coord.GetSource(); }
	
protected:
	PatchTextureLite1(const PatchTextureLite1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual void ImplementRender(ITextureRender* render) const override { m_coord.Render(render); }
	virtual bool ImplementChangeSize(const XMUINT2& size, bool) noexcept override { return UIComponent::ImplementChangeSize(size); }

private:
	PatchTextureCoord m_coord;
};