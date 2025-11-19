#include "pch.h"
#include "UIModule.h"
#include "IRenderer.h"
#include "Locator/InputLocator.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UIComponent/Traverser/UITraverser.h"
#include "UIComponent/Components/Panel.h"
#include "UserInterface/SerializerIO/ClassSerializeIO.h"
#include "UserInterface/Input/IMouseEventReceiver.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/StlExt.h"

using namespace UITraverser;

UIModule::~UIModule() = default;
UIModule::UIModule() :
	m_nameGen{ make_unique<UINameGenerator>() }
{}

bool UIModule::operator==(const UIModule& other) const noexcept
{
	ReturnIfFalse(Compare(m_nameGen, other.m_nameGen));
	ReturnIfFalse(Compare(m_mainPanel, other.m_mainPanel));

	return true;
}

bool UIModule::SetupMainComponent(const UILayout& layout, const string& name,
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_resBinder = CreateTextureResourceBinder(srcBinderFilename, renderer);
	ReturnIfFalse(m_resBinder);
	m_renderer = renderer;

	m_mainPanel = CreateComponent<Panel>(layout);
	m_mainPanel->SetUIModule(this);
	return Rename(m_mainPanel.get(), name);
}

bool UIModule::SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_resBinder = CreateTextureResourceBinder(srcBinderFilename, renderer);
	m_renderer = renderer;

	ReturnIfFalse(Read(filename));
	ReturnIfFalse(BindTextureResources());

	return true;
}

bool UIModule::BindTextureResources() noexcept
{
	ReturnIfFalse(m_resBinder);
	ReturnIfFalse(BindTextureSourceInfo(m_mainPanel.get(), m_resBinder.get(), m_renderer->GetTextureController()));

	return true;
}

bool UIModule::Update(const DX::StepTimer& timer) noexcept
{
	UpdateMouseState();
	return m_mainPanel->ProcessUpdate(timer);
}

void UIModule::Render(ITextureRender* render) const
{
	UITraverser::Render(m_mainPanel.get(), render);
}

void UIModule::UpdateMouseState() noexcept
{
	//hover는 공통으로 호출
	auto input = InputLocator::GetService();
	auto mouseState = input->GetMouseState();
	auto receivers = PickMouseReceivers(m_mainPanel.get(), mouseState.pos);

	UpdateHoverState(receivers, mouseState.pos);
	ProcessCaptureComponent(mouseState); //캡쳐된 컴포넌트를 마우스 입력에 따라 처리
	CaptureComponent(mouseState);	//클릭하면 캡쳐하고 press호출
	ProcessMouseWheel(input->GetMouseWheelValue());
}

void UIModule::UpdateHoverState(vector<IMouseEventReceiver*> receivers, const XMINT2& pos) noexcept
{
	for (auto& receiver : receivers)
	{
		receiver->OnHover(); //일단 다 호출하고나서
		receiver->OnMove(pos);
	}

	for (auto& prevComp : m_hoveredReceivers)
	{
		if (ranges::find(receivers, prevComp) == receivers.end())
			prevComp->OnNormal(); //영역이 아닌 애들은 OnNormal 호출
	}

	m_hoveredReceivers = receivers;
}

void UIModule::ProcessCaptureComponent(const MouseState& mouseState) noexcept
{
	if (!m_capture) return;

	bool inside = (ranges::find(m_hoveredReceivers, m_capture) != m_hoveredReceivers.end()); //hovered에서 찾으면 inside.
	if (mouseState.leftButton == InputState::Released) //3. 마우스를 떼면 release호출하고 캡쳐해제
	{
		m_capture->OnRelease(inside);
		m_capture = nullptr;
	}

	if (mouseState.leftButton == InputState::Held)
		m_capture->OnHold(mouseState.pos, inside); //2. 캡쳐한걸 hold로 호출한다.
}

static inline bool IsHandled(InputResult result) noexcept
{
	return result == InputResult::Consumed || result == InputResult::Propagate;
}

void UIModule::CaptureComponent(const MouseState& mouseState) noexcept
{
	if (m_capture) return;
	if (mouseState.leftButton != InputState::Pressed) return;

	for (auto& receiver : m_hoveredReceivers)
	{
		const auto result = receiver->OnPress(mouseState.pos);
		if (!IsHandled(result)) continue; //처리되지 않았다면 Render상 밑의 컴포넌트

		if (result == InputResult::Consumed) //이 컴포넌트가 소비하고 다음 컴포넌트로 가지 않는다.
		{
			m_capture = receiver;
			break;
		}

		if (!m_capture && result == InputResult::Propagate)
			m_capture = receiver; //캡쳐된게 없으면 일단 얘를 캡쳐
	}
}

void UIModule::ProcessMouseWheel(int wheelValue) noexcept
{
	if (!wheelValue) return;

	for (const auto& receiver : m_hoveredReceivers)
		if (receiver->OnWheel(wheelValue))
			return; //위의 컴포넌트가 반응하면 그 밑에 컴포넌트들은 실행하지 않는다.
}

void UIModule::ReloadDatas() noexcept
{
	m_mainPanel->SetUIModule(this);
}

void UIModule::ProcessIO(SerializerIO& serializer)
{
	serializer.Process("MainPanel", m_mainPanel);
	serializer.Process("UINameGenerator", m_nameGen);

	if (serializer.IsWrite()) return;
	ReloadDatas();
}

bool UIModule::Write(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	SerializerIO::WriteJsonToFile(*this, curFilename);
	m_filename = curFilename;

	return true;
}

bool UIModule::Read(const wstring& filename) noexcept
{
	const wstring& curFilename = !filename.empty() ? filename : m_filename;
	SerializerIO::ReadJsonFromFile(curFilename, *this);
	PropagateRoot(m_mainPanel.get()); //모든 컴포넌트들에 root를 지정.
	m_filename = curFilename;

	return true;
}

bool UIModule::EnableToolMode(bool enable) noexcept
{
	return UITraverser::EnableToolMode(m_mainPanel.get(), enable);
}

Panel* UIModule::GetMainPanel() const noexcept { return m_mainPanel.get(); }