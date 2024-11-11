#pragma once
#include "UIComponent.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;

enum class Origin;

class BGImage : public UIComponent
{
public:
	virtual ~BGImage();
	BGImage();
	BGImage(const BGImage& other);

	virtual unique_ptr<UIComponent> Clone() override;
	virtual bool ReadProperty(const nlohmann::json& data) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* mouseTracker) noexcept;
	virtual void Render(IRender* renderer) override;

	bool ChangeArea(const Rectangle& area) noexcept;
	void SetPosition(const Vector2& pos) noexcept;
	void SetImage(const string& name, const Vector2 position, const UILayout& layout, const ImageSource& sources);
	
private:
	Vector2 m_position{};
	unique_ptr<ImagePartSet> m_imagePartSet;
};
