#pragma once
#include "UIComponent.h"

struct IRenderer;
struct ImageSource;
class ImagePart;

class ImageGrid1 : public UIComponent
{
protected:
	ImageGrid1(const ImageGrid1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;

public:
	~ImageGrid1();
	ImageGrid1();

	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool ChangeArea(const Rectangle& area) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);
	inline Rectangle GetSource() const noexcept { return m_source; }

private:
	size_t m_index{ 0 };
	std::wstring m_filename{};

	Rectangle m_source{};
	Rectangle m_destination{};
	XMINT2 m_position{};
};

unique_ptr<ImageGrid1> CreateImageGrid1(const string& name, const UILayout& layout, const ImageSource& source);
