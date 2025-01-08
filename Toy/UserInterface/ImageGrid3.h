#pragma once
#include "UIComponent.h"

struct ImageSource;
struct SourceDivider;
class ImageGrid1;

class ImageGrid3 : public UIComponent
{
protected:
	ImageGrid3(const ImageGrid3& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~ImageGrid3();
	ImageGrid3();

	virtual void ChangeSize(const XMUINT2& size) noexcept override;

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);
	Rectangle GetFirstComponentSource() const noexcept;

	bool SetFilename(const wstring& filename) noexcept;
	bool GetFilename(wstring& outFilename) const noexcept;

	bool SetSourceAnd2Divider(const SourceDivider& srcDivider) noexcept;
	bool GetSourceAnd2Divider(SourceDivider& outSrcDivider) const noexcept;

	bool SetSources(const vector<Rectangle>& sources) noexcept;
	vector<Rectangle> GetSources() const noexcept;
	Rectangle GetMergedSource() const noexcept;
};