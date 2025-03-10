#pragma once
#include "../UIComponent.h"

class ImageChanger : public UIComponent
{
public:
	~ImageChanger();
	ImageChanger();

	static ComponentID GetTypeStatic() { return ComponentID::ImageChanger; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, const map<InteractState, ImageSource>& sources);

protected:
	ImageChanger(const ImageChanger& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
};

unique_ptr<ImageChanger> CreateImageChanger(const UILayout& layout,
	const map<InteractState, ImageSource>& sources);