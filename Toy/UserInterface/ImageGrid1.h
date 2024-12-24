#pragma once
#include "UIComponent.h"

template <typename T>
	requires std::is_default_constructible_v<T>&& std::is_copy_assignable_v<T>
class Property 
{
public:
	Property() = default;
	Property(const T& initialValue) : value(initialValue) {}
	bool operator==(const Property<T>& rhs) const noexcept { return value == rhs.value; }

	const T& Get() const { return value; }
	void Set(const T& newValue) { value = newValue; }

	operator T() const { return value; }
	Property& operator=(const T& newValue) 
	{
		Set(newValue);
		return *this;
	}

private:
	T value;
};

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
	virtual void SerializeIO(JsonOperation& operation) override;

	bool SetImage(const string& name, const UILayout& layout, const ImageSource& source);

public:
	Property<wstring> Filename{};
	Property<Rectangle> Source{};

private:
	size_t m_index{ 0 };
	XMINT2 m_position{};
};

unique_ptr<ImageGrid1> CreateImageGrid1(const string& name, const UILayout& layout, const ImageSource& source);
