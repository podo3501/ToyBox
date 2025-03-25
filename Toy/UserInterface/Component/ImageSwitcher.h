#pragma once
#include "../UIComponent.h"

enum class KeyState;
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

	bool Setup(const UILayout& layout, unique_ptr<ImageGrid> img, 
		const map<InteractState, ImageSource>& sources, BehaviorMode behaviorMode) noexcept;
	bool Setup(const UILayout& layout, ImagePart imgPart, 
		const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode);
	void AddPressCB(function<void(KeyState)> callback) { m_onPressCB = callback; }
	void ClearInteraction() noexcept;

protected:
	ImageSwitcher(const ImageSwitcher& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementLoadResource(ITextureLoad* load) override;
	virtual bool ImplementPostLoaded(ITextureController* texController) override;
	virtual bool ImplementBindSourceInfo(TextureSourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void Release() noexcept;
	void ReloadDatas() noexcept;
	void AddRef() const noexcept;
	void NormalMode(bool isPressed, bool isHeld) noexcept;
	void HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept;
	void SetState(InteractState state) noexcept;

	map<InteractState, string> m_stateKeys;
	BehaviorMode m_behaviorMode{ BehaviorMode::Normal };

	map<InteractState, ImageSource> m_sources; //?!?저장항목에서 삭제
	map<InteractState, size_t> m_indexes;
	ImageGrid* m_image;
	optional<InteractState> m_state;
	function<void(KeyState)> m_onPressCB;
	ITextureController* m_texController;
};

unique_ptr<ImageSwitcher> CreateImageSwitcher(const UILayout& layout,
	unique_ptr<ImageGrid> img, const map<InteractState, ImageSource>& sources, BehaviorMode behaviorMode);

unique_ptr<ImageSwitcher> CreateImageSwitcher(const UILayout& layout, ImagePart imgPart,
	const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode);