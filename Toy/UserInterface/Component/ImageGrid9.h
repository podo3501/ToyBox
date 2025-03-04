#pragma once
#include "../UIComponent.h"

struct ImageSource;
struct SourceDivider;
class ImageGrid3;

class ImageGrid9 : public UIComponent
{
public:
	~ImageGrid9();
	ImageGrid9();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool ChangeSize(const XMUINT2& size) noexcept override;

	bool SetImage(const UILayout& layout, const ImageSource& source) noexcept;

	bool SetFilename(const wstring& filename) noexcept;
	optional<wstring> GetFilename() const noexcept;

	bool SetSourceAnd4Divider(const SourceDivider& srcDivider) noexcept;
	SourceDivider GetSourceAnd4Divider() const noexcept;

	bool SetSources(const vector<Rectangle>& sources) noexcept;
	vector<Rectangle> GetSources() const noexcept;

protected:
	ImageGrid9(const ImageGrid9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
};

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const ImageSource& source);