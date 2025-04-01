#pragma once
#include "../UIComponent.h"

struct ImageSource;
struct SourceDivider;
struct TextureSourceInfo;
class ImageGrid3;

class ImageGrid9 : public UIComponent
{
public:
	~ImageGrid9();
	ImageGrid9();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid9; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }

	bool Setup(const UILayout& layout, const string& bindKey);
	const string& GetBindKey() const noexcept;
	void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept;

protected:
	ImageGrid9(const ImageGrid9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureSourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;
};

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const string& bindKey);