#pragma once

class PatchTextureBase;
class UIComponent;
class JsonOperation;
enum class DirectionType;

class PatchTextureImpl
{
public:
	~PatchTextureImpl();
	PatchTextureImpl() noexcept;
	PatchTextureImpl(const PatchTextureImpl& o) noexcept;
	bool operator==(const PatchTextureImpl& rhs) const noexcept;
	//PatchTextureBase
	void SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept;
	bool FitToTextureSource() noexcept;

	bool ChangeSize(const XMUINT2& size, const vector<UIComponent*>& components) noexcept;
	void SerializeIO(JsonOperation& operation);

	inline void SetOwner(PatchTextureBase* component) noexcept { m_component = component; }
	inline void SetDirectionType(DirectionType dirType) noexcept { m_dirType = dirType; }
	inline DirectionType GetDirectionType() const  noexcept { return m_dirType; }

private:
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;
	bool ForEachTexture(predicate<PatchTextureBase*, size_t> auto&& each);

	PatchTextureBase* m_component;
	DirectionType m_dirType;
};
