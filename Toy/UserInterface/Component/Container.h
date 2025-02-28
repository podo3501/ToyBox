#pragma once
#include "../UIComponent.h"

enum class KeyState;
namespace DX { class StepTimer; }

class Container : public UIComponent
{
public:
	~Container();
	Container() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::Container; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	void AddPressCB(function<void(KeyState)> callback) { m_onPressCB = callback; }
	bool Setup(const UILayout& layout, 
		map<InteractState, unique_ptr<UIComponent>> imgGridList, BehaviorMode behaviorMode) noexcept;
	void ClearInteraction() noexcept;
	const optional<InteractState>& GetState() const noexcept { return m_state; }

protected:
	Container(const Container& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementPostLoaded(ITextureController*) override;
	virtual bool ImplementUpdatePosition(const DX::StepTimer& timer, const XMINT2& absolutePos) noexcept override;
	virtual bool ImplementActiveUpdate() noexcept override;

private:
	void ReloadDatas() noexcept;
	void SetState(InteractState state) noexcept;
	void AttachComponent(InteractState state, unique_ptr<UIComponent>&& component) noexcept;
	bool NormalMode(bool isPressed, bool isHeld) noexcept;
	bool HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept;

	map<InteractState, UIComponent*> m_images;
	optional<InteractState> m_state;
	Rectangle m_area{};

	BehaviorMode m_behaviorMode{ BehaviorMode::Normal };
	function<void(KeyState)> m_onPressCB;
};

unique_ptr<Container> CreateContainer(const UILayout& layout, map<InteractState, unique_ptr<UIComponent>> imgGridList, BehaviorMode behaviorMode);