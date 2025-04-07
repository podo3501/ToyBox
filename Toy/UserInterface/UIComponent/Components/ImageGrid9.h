#pragma once
#include "ImageGrid.h"

struct SourceDivider;
struct TextureSourceInfo;
class ImageGrid3;

class ImageGrid9 : public ImageGrid
{
public:
	~ImageGrid9();
	ImageGrid9();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }

	bool Setup(const UILayout& layout, const string& bindKey);
	void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept;
	bool FitToTextureSource() noexcept override;
	const string& GetBindKey() const noexcept;

protected:
	ImageGrid9(const ImageGrid9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	bool ForEachImageGrid3(predicate<ImageGrid3*, size_t> auto&& each);
	bool ApplyStretchSize(const vector<XMUINT2>& sizes) noexcept;
	bool ApplyPositions(const XMUINT2& size, vector<XMUINT2>& sizes) noexcept;
};

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const string& bindKey);