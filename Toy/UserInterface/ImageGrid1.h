#pragma once
#include "UIComponent.h"

struct IRenderer;
struct ImageSource;
class ImagePart;

class ImageGrid1 : public UIComponent
{
public:
	~ImageGrid1();
	ImageGrid1();
	ImageGrid1(const ImageGrid1& other) = default;

	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept override;
	virtual void Render(IRender* render) override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);

private:
	size_t m_index{ 0 };
	std::wstring m_filename{};

	Rectangle m_source{};
	Rectangle m_destination{};
	XMINT2 m_position{};
};

unique_ptr<ImageGrid1> CreateGrid1(const string& name, const UILayout& layout, const ImageSource& source);
