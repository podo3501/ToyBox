#pragma once
#include "../UIComponent.h"

enum class KeyState;
struct TextureSourceInfo;
class ImageGrid;
namespace DX { class StepTimer; }
class ImageSwitcher : public UIComponent
{
public:
	~ImageSwitcher();
	ImageSwitcher();

	static ComponentID GetTypeStatic() { return ComponentID::ImageSwitcher; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, ImagePart imgPart, 
		const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode);
	void AddPressCB(function<void(KeyState)> callback) { m_onPressCB = callback; }
	void ClearInteraction() noexcept;
	bool FitToTextureSource() noexcept;

protected:
	ImageSwitcher(const ImageSwitcher& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void ReloadDatas() noexcept;
	void NormalMode(bool isPressed, bool isHeld) noexcept;
	void HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept;
	void SetState(InteractState state) noexcept;

	map<InteractState, string> m_stateKeys;
	BehaviorMode m_behaviorMode{ BehaviorMode::Normal };

	map<InteractState, TextureSourceInfo> m_sources;
	map<InteractState, size_t> m_indexes;
	ImageGrid* m_image;
	optional<InteractState> m_state;
	function<void(KeyState)> m_onPressCB;
};

unique_ptr<ImageSwitcher> CreateImageSwitcher(const UILayout& layout, ImagePart imgPart,
	const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode);