#pragma once
#include "PatchTextureLite.h"
#include "../PatchTextureImpl.h"

class PatchTextureLite1;
enum class DirectionType;

class PatchTextureLite3 : public PatchTextureLite
{
public:
	~PatchTextureLite3();
	PatchTextureLite3() noexcept;
	explicit PatchTextureLite3(DirectionType dirType) noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureLite3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;
	//PatchTextureLite
	virtual bool SetupLayout(size_t index, const vector<Rectangle>& sources, const XMUINT2& size) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override;

	Rectangle GetFirstComponentSource() const noexcept;

protected:
	PatchTextureLite3(const PatchTextureLite3& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual vector<Rectangle> GetChildSourceList() const noexcept override;

private:
	void SetDirectionType(DirectionType dirType) noexcept;

	DirectionType m_dirType;
	PatchTextureImpl m_impl;
};
