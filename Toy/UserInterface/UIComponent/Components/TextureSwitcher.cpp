#include "pch.h"
#include "TextureSwitcher.h"
#include "PatchTexture/PatchTextureLite/PatchTextureLite.h"
#include "../../TextureResourceBinder/TextureResourceBinder.h"
#include "../Include/IRenderer.h"
#include "InputManager.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "Utility.h"

using enum InteractState;

TextureSwitcher::~TextureSwitcher() = default;
TextureSwitcher::TextureSwitcher() :
	m_patchTexL{ nullptr }
{}

TextureSwitcher::TextureSwitcher(const TextureSwitcher& o) :
	UIComponent{ o },
	m_stateKeys{ o.m_stateKeys },
	m_behaviorMode{ o.m_behaviorMode },
	m_sources{ o.m_sources },
	m_indexes{ o.m_indexes },
	m_patchTexL{ nullptr },
	m_state{ o.m_state }
{
	ReloadDatas();
}

void TextureSwitcher::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_patchTexL = static_cast<PatchTextureLite*>(componentList[0]);
}

unique_ptr<UIComponent> TextureSwitcher::CreateClone() const
{
	return unique_ptr<TextureSwitcher>(new TextureSwitcher(*this));
}

bool TextureSwitcher::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const TextureSwitcher* o = static_cast<const TextureSwitcher*>(&rhs);

	return tie(m_stateKeys, m_behaviorMode) == tie(o->m_stateKeys, o->m_behaviorMode);
}

bool TextureSwitcher::Setup(const UILayout& layout, TextureSlice texSlice,
	const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode)
{
	SetLayout(layout);
	unique_ptr<PatchTextureLite> pTexL = nullptr;
	tie(pTexL, m_patchTexL) = GetPtrs(CreatePatchTextureLite(texSlice));
	UIEx(this).AttachComponent(move(pTexL), {});
	m_stateKeys = stateKeys;
	m_behaviorMode = behaviorMode;

	return true;
}

void TextureSwitcher::ClearInteraction() noexcept
{
	if (m_state && m_state == InteractState::Hovered)
		ChangeState(InteractState::Normal);
}

bool TextureSwitcher::SetSourceInfo(TextureResourceBinder* resBinder, InteractState state, const string& bindKey) noexcept
{
	auto sourceInfoRef = resBinder->GetTextureSourceInfo(bindKey);
	ReturnIfFalse(sourceInfoRef);

	const auto& srcInfo = sourceInfoRef->get();
	auto curIndex = srcInfo.GetIndex();
	ReturnIfFalse(curIndex);

	//바로 indexedSource 입력하는게 아니라 binder와 키값과 인덱스를 넘겨줘서 PatchTexture 내용까지 다 바꿔야 하지 않을까?
	m_indexes.insert_or_assign(state, *curIndex);
	m_sources.insert_or_assign(state, srcInfo);

	return true;
}

bool TextureSwitcher::SetupDefaults() noexcept
{
	InteractState defaultState = Normal;

	auto it = m_sources.find(defaultState);
	if (it == m_sources.end()) return false;

	const auto& srcInfo = it->second;
	const auto optIndex = srcInfo.GetIndex();
	ReturnIfFalse(optIndex);

	ReturnIfFalse(m_patchTexL->SetupLayout(*optIndex, srcInfo.sources, GetSize()));
	m_state = defaultState;

	return true;
}

bool TextureSwitcher::ImplementBindSourceInfo(TextureResourceBinder* resBinder, ITextureController*) noexcept
{
	for (const auto& pair : m_stateKeys)
		ReturnIfFalse(SetSourceInfo(resBinder, pair.first, pair.second));	

	return SetupDefaults();
}

bool TextureSwitcher::ChangeBindKey(TextureResourceBinder* resBinder, const string& bindKey) noexcept
{
	if (!m_state) return false;
	auto it = m_stateKeys.find(*m_state);
	it->second = bindKey;

	SetSourceInfo(resBinder, *m_state, bindKey);
	SetState(*m_state);

	return true;
}

void TextureSwitcher::NormalMode(bool isPressed, bool isHeld) noexcept
{
	if (!Contains(GetArea(), InputManager::GetMouse().GetPosition()))
	{
		ChangeState(Normal);
		return;
	}

	ChangeState((isPressed || (m_state == Pressed && isHeld)) ? Pressed : Hovered);
}

//마우스 왼쪽키가 계속 눌러지고 있으면 영역을 벗어나도 눌러지는 state가 유지된다.(scrollbar에서 사용)
void TextureSwitcher::HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept
{
	if (m_state == Pressed && isHeld) //Pressed가 계속 되고 있다면 리턴한다.
	{
		m_onPressCB(KeyState::Held);
		return;
	}

	NormalMode(isPressed, isHeld);

	if (m_state == Pressed && isPressed) m_onPressCB(KeyState::Pressed);
}

bool TextureSwitcher::ImplementUpdate(const DX::StepTimer&) noexcept
{
	if (!m_state.has_value()) return true; //로드 하지 않았다면 값이 셋팅되지 않는다.
	//이 두값이 이전프레임과 비교해서 달라졌다면 실행하게 한다면 좀 더 빠르게 된다.
	bool isPressed = IsInputAction(MouseButton::Left, KeyState::Pressed);
	bool isHeld = IsInputAction(MouseButton::Left, KeyState::Held);

	switch (m_behaviorMode) //이 부분은 배열에 함수포인터로 하면 더 빨라지는데 추후 다양한 behavior가 생기면 인자가 달라질수 있기 때문에 일단 보류한다.
	{
	case BehaviorMode::Normal: NormalMode(isPressed, isHeld); break;
	case BehaviorMode::HoldToKeepPressed: HoldToKeepPressedMode(isPressed, isHeld); break;
	}

	return true;
}

bool TextureSwitcher::ImplementChangeSize(const XMUINT2& size) noexcept
{
	bool allChange = ranges::all_of(GetChildComponents(), [&size](const auto& component) {
		return component->ChangeSize(size);
		});
	ReturnIfFalse(allChange);
	return UIComponent::ImplementChangeSize(size);
}

optionalRef<string> TextureSwitcher::GetBindKey() const noexcept
{
	if (!m_state) return nullopt;
	auto it = m_stateKeys.find(*m_state);
	if (it == m_stateKeys.end()) return nullopt;

	return cref(it->second);
}

bool TextureSwitcher::FitToTextureSource() noexcept
{
	ReturnIfFalse(m_patchTexL->FitToTextureSource());
	SetSize(m_patchTexL->GetSize());

	return true; 
}

void TextureSwitcher::SetState(InteractState state) noexcept
{	
	m_patchTexL->SetIndexedSource(m_indexes[state], m_sources[state].sources);
	m_state = state;
}

void TextureSwitcher::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("StateKey", m_stateKeys);
	operation.Process("BehaviorMode", m_behaviorMode);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

//////////////////////////////////////////////////////

//create
unique_ptr<TextureSwitcher> CreateTextureSwitcher(const UILayout& layout, TextureSlice texSlice,
	const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode)
{
	unique_ptr<TextureSwitcher> switcher = make_unique<TextureSwitcher>();
	return CreateIfSetup(move(switcher), layout, texSlice, stateKeys, behaviorMode);
}

//utility
static inline PatchTextureLite* GetPTexL(TextureSwitcher* s) noexcept { return s->GetPatchTextureLite(); }
optional<TextureSlice> GetTextureSlice(TextureSwitcher* s) noexcept { return GetPTexL(s)->GetTextureSlice(); }