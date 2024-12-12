#pragma once
#include "UIComponent.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;
class JsonOperation;

enum class Origin;

class BGImage : public UIComponent
{
public:
	virtual ~BGImage();
	BGImage();
	BGImage(const BGImage& other);
	virtual bool operator==(const UIComponent& o) const noexcept override;

	virtual unique_ptr<UIComponent> Clone() override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept;
	virtual void Render(IRender* renderer) override;
	virtual bool ChangeArea(const Rectangle& area) noexcept override;

	void SetImage(const string& name, const UILayout& layout, const ImageSource& sources);
	void GetImageSource() const noexcept;

	virtual void SerializeIO(JsonOperation& operation) override;
	
private:
	unique_ptr<ImagePartSet> m_imagePartSet;
};
