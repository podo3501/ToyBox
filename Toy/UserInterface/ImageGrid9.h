#pragma once
#include "UIComponent.h"

struct ImageSource;
struct SourceDivider;
class ImageGrid3;

class ImageGrid9 : public UIComponent
{
protected:
	ImageGrid9(const ImageGrid9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~ImageGrid9();
	ImageGrid9();

	virtual void ChangeSize(const XMUINT2& size) noexcept override;

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);

	bool SetFilename(const wstring& filename) noexcept;
	bool GetFilename(wstring& outFilename) const noexcept;

	bool SetSourceAnd4Divider(const SourceDivider& srcDivider) noexcept;
	bool GetSourceAnd4Divider(SourceDivider& outSrcDivider) const noexcept;

	bool SetSources(const vector<Rectangle>& sources) noexcept;
	vector<Rectangle> GetSources() const noexcept;

private:
	
};