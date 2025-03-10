#include "pch.h"
#include "ImageChanger.h"

ImageChanger::~ImageChanger() = default;
ImageChanger::ImageChanger()
{}

ImageChanger::ImageChanger(const ImageChanger& o) :
	UIComponent{ o }
{}

unique_ptr<UIComponent> ImageChanger::CreateClone() const
{
	return unique_ptr<ImageChanger>(new ImageChanger(*this));
}

bool ImageChanger::Setup(const UILayout& layout, const map<InteractState, ImageSource>& sources)
{
	SetLayout(layout);
	sources;

	return true;
}

void ImageChanger::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
}

unique_ptr<ImageChanger> CreateImageChanger(const UILayout& layout,
	const map<InteractState, ImageSource>& sources)
{
	unique_ptr<ImageChanger> imgChanger = make_unique<ImageChanger>();
	return imgChanger->Setup(layout, sources) ? move(imgChanger) : nullptr;
}