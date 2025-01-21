#pragma once
#include "../UIComponent.h"

struct ImageSource;
struct SourceDivider;
class ImageGrid1;

class ImageGrid3 : public UIComponent
{
public:
	~ImageGrid3();
	ImageGrid3();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid3; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual void ChangeSize(const XMUINT2& size) noexcept override;

	bool SetImage(const UILayout& layout, const ImageSource& source) noexcept;
	Rectangle GetFirstComponentSource() const noexcept;

	bool SetFilename(const wstring& filename) noexcept;
	bool GetFilename(wstring& outFilename) const noexcept;

	bool SetSourceAnd2Divider(const SourceDivider& srcDivider) noexcept;
	bool GetSourceAnd2Divider(SourceDivider& outSrcDivider) const noexcept;

	bool SetSources(const vector<Rectangle>& sources) noexcept;
	vector<Rectangle> GetSources() const noexcept;
	Rectangle GetMergedSource() const noexcept;

protected:
	ImageGrid3(const ImageGrid3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
};