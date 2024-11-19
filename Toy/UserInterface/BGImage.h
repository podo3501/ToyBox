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
	//virtual bool ReadProperty(const nlohmann::json& data) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* mouseTracker) noexcept;
	virtual void Render(IRender* renderer) override;

	bool ChangeArea(const Rectangle& area) noexcept;
	void SetImage(const string& name, const UILayout& layout, const ImageSource& sources);

	friend void to_json(nlohmann::ordered_json& j, const BGImage& data);
	friend void from_json(const nlohmann::json& j, BGImage& data);

	virtual void SerializeIO(JsonOperation* operation) override;
	
private:
	unique_ptr<ImagePartSet> m_imagePartSet;
};
