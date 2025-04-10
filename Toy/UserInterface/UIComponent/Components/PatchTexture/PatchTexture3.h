#pragma once
#include "PatchTexture.h"

struct SourceDivider;
struct TextureSourceInfo;
class PatchTexture1;
enum class DirectionType;

class PatchTexture3 : public PatchTexture
{
public:
	~PatchTexture3();
	PatchTexture3() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::PatchTexture3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;
	//PatchTexture
	virtual bool FitToTextureSource() noexcept override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept override;
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept override;

	bool Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept;
	const string& GetBindKey() const noexcept;
	void ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept;
	Rectangle GetFirstComponentSource() const noexcept;
	inline DirectionType GetDirectionType() const noexcept { return m_dirType; }

protected:
	PatchTexture3(const PatchTexture3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	bool ForEachPatchTexture1(predicate<PatchTexture1*, size_t> auto&& each);
	void SetDirectionType(DirectionType dirType) noexcept;
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;

	DirectionType m_dirType{ DirectionType::Horizontal };
};

unique_ptr<PatchTexture3> CreatePatchTexture3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex = 0u);