#pragma once
#include "../UIComponent.h"

struct IRenderer;
struct ImageSource;
class ImagePart;

class ImageGrid1 : public UIComponent
{
public:
	~ImageGrid1();
	ImageGrid1();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool SetImage(const UILayout& layout, const ImageSource& source) noexcept;

protected:
	ImageGrid1(const ImageGrid1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementUpdate(const XMINT2& position) noexcept override;
	virtual void ImplementRender(IRender* render) const override;

public:
	Property<wstring> Filename{};
	Property<Rectangle> Source{};

	void SetSource(const Rectangle& source) noexcept { Source = source; }
	const Rectangle& GetSource() const noexcept { return Source.Get(); }

private:
	size_t m_index{ 0 };
	XMINT2 m_position{};
};
