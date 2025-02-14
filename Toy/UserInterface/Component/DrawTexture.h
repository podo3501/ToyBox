#pragma once
#include "../UIComponent.h"

class DrawTexture : public UIComponent
{
public:
	~DrawTexture();
	DrawTexture();

	static ComponentID GetTypeStatic() { return ComponentID::DrawTexture; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool CreateTexture(IGetValue* resourceInfo, const XMUINT2& size, UIComponent* component, ImTextureID* texID);

protected:
	DrawTexture(const DrawTexture& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementUpdatePosition(const XMINT2& position) noexcept override;
	virtual void ImplementRender(IRender* render) const override;

private:
	size_t m_index{ 0 };
	IGetValue* m_resourceInfo;
};