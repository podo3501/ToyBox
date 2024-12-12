#pragma once
#include "UIComponent.h"

struct IRenderer;
class ImagePart;

class SingleImage : public UIComponent
{
public:
	~SingleImage();
	SingleImage();
	SingleImage(const SingleImage& other);
	virtual bool operator==(const UIComponent& o) const noexcept override;

	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept override;
	virtual void Render(IRender* render) override;

	void SetImage(const string& name, const UILayout& layout, const wstring& filename);

	virtual void SerializeIO(JsonOperation& operation) override;

private:
	unique_ptr<ImagePart> m_imagePart;
};
