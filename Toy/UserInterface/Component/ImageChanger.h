#pragma once
#include "../UIComponent.h"

enum class KeyState;
class ImageGrid;
namespace DX { class StepTimer; }
class ImageChanger : public UIComponent
{
public:
	~ImageChanger();
	ImageChanger();

	static ComponentID GetTypeStatic() { return ComponentID::ImageChanger; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, unique_ptr<ImageGrid> img, 
		const map<InteractState, ImageSource>& sources, BehaviorMode behaviorMode) noexcept;
	void AddPressCB(function<void(KeyState)> callback) { m_onPressCB = callback; }

protected:
	ImageChanger(const ImageChanger& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementLoadResource(ITextureLoad* load) override;
	virtual bool ImplementPostLoaded(ITextureController* texController) override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void Release() noexcept;
	void AddRef() const noexcept;
	void NormalMode(bool isPressed, bool isHeld) noexcept;
	void HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept;
	void SetState(InteractState state) noexcept;

	map<InteractState, ImageSource> m_sources;
	map<InteractState, IndexedSource> m_indexedSources;
	ImageGrid* m_image;
	optional<InteractState> m_state;
	BehaviorMode m_behaviorMode{ BehaviorMode::Normal };
	function<void(KeyState)> m_onPressCB;
	ITextureController* m_texController;
};

unique_ptr<ImageChanger> CreateImageChanger(const UILayout& layout, 
	unique_ptr<ImageGrid> img, const map<InteractState, ImageSource>& sources, BehaviorMode behaviorMode);