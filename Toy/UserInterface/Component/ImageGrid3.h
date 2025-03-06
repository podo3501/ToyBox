#pragma once
#include "../UIComponent.h"

struct ImageSource;
struct SourceDivider;
class ImageGrid1;
enum class DirectionType;

class ImageGrid3 : public UIComponent
{
public:
	~ImageGrid3();
	ImageGrid3();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool SetImage(DirectionType dirType, const UILayout& layout, const ImageSource& source) noexcept;
	Rectangle GetFirstComponentSource() const noexcept;

	bool SetFilename(const wstring& filename) noexcept;
	optional<wstring> GetFilename() const noexcept;

	bool SetSourceAnd2Divider(const SourceDivider& srcDivider) noexcept;
	SourceDivider GetSourceAnd2Divider() const noexcept;

	bool SetSources(const vector<Rectangle>& sources) noexcept;
	vector<Rectangle> GetSources() const noexcept;
	Rectangle GetMergedSource() const noexcept;
	inline DirectionType GetDirectionType() const noexcept { return m_dirType; }

protected:
	ImageGrid3(const ImageGrid3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	DirectionType m_dirType{ DirectionType::Horizontal };
};

unique_ptr<ImageGrid3> CreateImageGrid3(DirectionType dirType, const UILayout& layout, const ImageSource& source);