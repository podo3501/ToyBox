#pragma once
#include "PatchTextureLite.h"

class PatchTextureLite1 : public PatchTextureLite
{
public:
	~PatchTextureLite1();
	PatchTextureLite1();

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	//PatchTextureLite
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override;
	virtual bool FitToTextureSource() noexcept override;

	bool Setup() noexcept;

protected:
	PatchTextureLite1(const PatchTextureLite1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	optional<size_t> m_index; //텍스쳐 인덱스. 0값도 인덱스로 사용하기 때문에 optional
	Rectangle m_source{};
};