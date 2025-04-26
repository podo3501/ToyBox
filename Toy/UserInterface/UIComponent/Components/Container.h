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
	virtual void SerializeIO(JsonOperation& operation) override;

	void AddPressCB(function<void(KeyState)> callback) { m_onPressCB = callback; }
	bool Setup(const UILayout& layout, 
		map<InteractState, unique_ptr<UIComponent>> patchTexList, BehaviorMode behaviorMode) noexcept;
	void ClearInteraction() noexcept;
	const optional<InteractState>& GetState() const noexcept { return m_state; }

protected:
	Container(const Container& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept;

private:
	void ReloadDatas() noexcept;
	void SetState(InteractState state) noexcept;
	void AttachComponent(InteractState state, unique_ptr<UIComponent>&& component) noexcept;
	void NormalMode(bool isPressed, bool isHeld) noexcept;
	void HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept;

	map<InteractState, UIComponent*> m_textures;
	optional<InteractState> m_state;

	BehaviorMode m_behaviorMode{ BehaviorMode::Normal };
	function<void(KeyState)> m_onPressCB;
};

unique_ptr<Container> CreateContainer(const UILayout& layout, map<InteractState, unique_ptr<UIComponent>> patchTexList, BehaviorMode behaviorMode);