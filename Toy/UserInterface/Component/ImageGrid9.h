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

	bool Setup(const UILayout& layout, const string& bindKey);
	wstring GetFilename() const noexcept;

	bool SetSourceAnd4Divider(const SourceDivider& srcDivider) noexcept;
	SourceDivider GetSourceAnd4Divider() const noexcept; //?!? 이렇게 자르는 함수들은 Binder에서 잘라서 저장했기 때문에 여기서는 할 필요가 없어졌다.

	bool SetSources(const vector<Rectangle>& sources) noexcept;
	vector<Rectangle> GetSources() const noexcept;

protected:
	ImageGrid9(const ImageGrid9& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureSourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

};

unique_ptr<ImageGrid9> CreateImageGrid9(const UILayout& layout, const string& bindKey);