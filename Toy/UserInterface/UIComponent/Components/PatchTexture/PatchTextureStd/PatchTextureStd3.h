#pragma once
#include "PatchTextureStd.h"
#include "../PatchTextureImpl.h"

struct TextureSourceInfo;
class PatchTextureStd1;
enum class DirectionType;

class PatchTextureStd3 : public PatchTextureStd
{
public:
	~PatchTextureStd3();
	PatchTextureStd3() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureStd3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;
	//PatchTexture
	virtual bool FitToTextureSource() noexcept override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept override;
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept override;
	virtual const string& GetBindKey() const noexcept override;

	bool Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept;
	void ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept;
	Rectangle GetFirstComponentSource() const noexcept;
	inline DirectionType GetDirectionType() const noexcept { return m_dirType; }

protected:
	PatchTextureStd3(const PatchTextureStd3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void SetDirectionType(DirectionType dirType) noexcept;

	DirectionType m_dirType;
	PatchTextureImpl m_impl;
};

unique_ptr<PatchTextureStd3> CreatePatchTextureStd3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex = 0u);