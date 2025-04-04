#pragma once
#include "ImageGrid.h"

struct SourceDivider;
struct TextureSourceInfo;
class ImageGrid1;
enum class DirectionType;

class ImageGrid3 : public ImageGrid
{
public:
	~ImageGrid3();
	ImageGrid3();
	ImageGrid3(DirectionType dirType);

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& sources) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex) noexcept;
	const string& GetBindKey() const noexcept;
	void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex = 0u) noexcept;
	Rectangle GetFirstComponentSource() const noexcept;
	inline DirectionType GetDirectionType() const noexcept { return m_dirType; }

protected:
	ImageGrid3(const ImageGrid3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	DirectionType m_dirType{ DirectionType::Horizontal };
};

unique_ptr<ImageGrid3> CreateImageGrid3(const UILayout& layout, DirectionType dirType, const string& bindKey, size_t sourceIndex = 0u);