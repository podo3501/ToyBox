#pragma once
#include "../UIComponent.h"

enum class InputState;
struct TextureSourceInfo;
class PatchTextureLite;
namespace DX { class StepTimer; }
class TextureSwitcher : public UIComponent
{
public:
	~TextureSwitcher();
	TextureSwitcher();

	static ComponentID GetTypeStatic() { return ComponentID::TextureSwitcher; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void ProcessIO(SerializerIO& serializer) override;
	virtual bool OnNormal() noexcept override;
	virtual bool OnHover() noexcept override;
	virtual bool OnPress(const XMINT2& pos) noexcept override;
	virtual bool OnHold(const XMINT2& pos, bool inside) noexcept override;
	virtual bool OnRelease(bool inside) noexcept override;

	bool Setup(const UILayout& layout, TextureSlice texSlice, const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode);
	inline bool Setup(TextureSlice texSlice, const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode) { return Setup({}, texSlice, stateKeys, behaviorMode); }
	void AddPressCB(function<void(const XMINT2&, InputState)> callback) { m_onPressCB = callback; }
	unique_ptr<UIComponent> AttachComponentToCenter(unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept;
	void ClearInteraction() noexcept;
	bool FitToTextureSource() noexcept;
	void ChangeState(InteractState state, bool force = false) noexcept;
	void ChangeBehaviorMode(BehaviorMode behaviorMode) noexcept { m_behaviorMode = behaviorMode; }
	bool ChangeBindKey(TextureResourceBinder* resBinder, const string& bindKey) noexcept;
	inline optional<InteractState> GetState() const noexcept 	{ return m_state; }
	inline PatchTextureLite* GetPatchTextureLite() const noexcept { return m_patchTexL; }
	string GetBindKey() const noexcept;

protected:
	TextureSwitcher(const TextureSwitcher& o);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept;

private:
	bool SetSourceInfo(TextureResourceBinder* resBinder, InteractState state, const string& bindKey) noexcept;
	bool SetupDefaults() noexcept;
	void ReloadDatas() noexcept;
	void SetState(InteractState state) noexcept;

	map<InteractState, string> m_stateKeys;
	BehaviorMode m_behaviorMode{ BehaviorMode::Normal };

	map<InteractState, TextureSourceInfo> m_sources;
	map<InteractState, size_t> m_indexes;
	PatchTextureLite* m_patchTexL;
	optional<InteractState> m_state;
	function<void(const XMINT2&, InputState)> m_onPressCB;
};

//utility
optional<TextureSlice> GetTextureSlice(TextureSwitcher* switcher) noexcept;