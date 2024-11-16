#include "pch.h"
#include "Button.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"
#include "JsonHelper.h"

string Button::GetStringState(State state)
{
	if (state == State::Normal) return "Normal";
	if (state == State::Hover) return "Hover";
	if (state == State::Pressed) return "Presssed";

	return "";
}

Button::State Button::GetStateString(const string& str)
{
	if (str == "Normal") return State::Normal;
	if (str == "Hover") return State::Hover;
	if (str == "Presssed") return State::Pressed;

	return State::Init;
}

Button::~Button() = default;
Button::Button() {}
Button::Button(const Button& other)
	: UIComponent{ other }
{
	ranges::for_each(other.m_image, [this](const auto& pair) {
		State state = pair.first;
		const auto& imgPartSet = pair.second;
		m_image.insert(make_pair(state, make_unique<ImagePartSet>(*imgPartSet.get())));
		});
}

unique_ptr<UIComponent> Button::Clone()
{
	return make_unique<Button>(*this);
}

//bool Button::ReadProperty(const nlohmann::json& data)
//{
//	for (const auto& strState : { "Normal", "Hover", "Pressed" })
//	{
//		const auto& stateData = data[strState];
//		vector<ImageSource> imgSrcList;
//		for (size_t i{ 0 }; stateData.size() != i; i++)
//		{
//			ImageSource imgSrc;
//			imgSrc.Read(stateData[i]);
//			imgSrcList.emplace_back(imgSrc);
//		}
//		InsertImage(GetStateString(strState), make_unique<ImagePartSet>(imgSrcList));
//	}
//
//	return true;
//}

bool Button::LoadResources(ILoadData* load)
{
	ReturnIfFalse(ranges::all_of(m_image | views::values, [this, load](const auto& imgPartSet) {
		return imgPartSet->LoadResources(load);
		}));

	//로딩을 다 하고 값을 넣는 이유는 뒤에 로딩하는 것들이 값에 영향을 주기 때문이다.
	auto layout = GetLayout();
	SetDestination(layout->GetArea());

	return true;
}

bool Button::SetDestination(const Rectangle& area) noexcept
{
	for (const auto& partSet : m_image | views::values)
		ReturnIfFalse(partSet->SetDestination(area));

	return true;
}

bool Button::ChangeArea(const Rectangle& area) noexcept
{
	ReturnIfFalse(SetDestination(area));

	UIComponent::ChangeArea(area);

	return true;
}

bool Button::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept
{
	auto layout = GetLayout();
	const Vector2& pos = layout->GetPosition(position);

	for (const auto& partSet : m_image | views::values)
		ReturnIfFalse(partSet->Update(pos));

	bool bHover = ranges::any_of(m_image | views::values, [mouseState = tracker->GetLastState()](const auto& partSet) {
		return partSet->IsHover(mouseState.x, mouseState.y);
		});

	if (!bHover)
	{
		m_state = State::Normal;
		return true;
	}

	if (tracker->leftButton == Mouse::ButtonStateTracker::PRESSED ||
		m_state == State::Pressed && tracker->leftButton == Mouse::ButtonStateTracker::HELD)
		m_state = State::Pressed;
	else
		m_state = State::Hover;

	return true;
}

void Button::Render(IRender* render)
{
	m_image[m_state]->Render(render);
}

void Button::SetImage(const string& name,
	const UILayout& layout,
	const vector<ImageSource>& normal,
	const vector<ImageSource>& hover,
	const vector<ImageSource>& pressed)
{
	SetName(name);
	SetLayout(layout);

	State btnState = State::Normal;
	for (const auto& sources : { normal, hover, pressed })
	{
		InsertImage(btnState, make_unique<ImagePartSet>(sources));
		btnState = static_cast<State>(static_cast<int>(btnState) + 1);
	}
}

void Button::ChangeOrigin(Origin&& origin) noexcept
{
	auto layout = GetLayout();
	layout->Set(move(origin));
}

void Button::InsertImage(State btnState, unique_ptr<ImagePartSet>&& imgPartSet)
{
	m_image.insert(make_pair(btnState, move(imgPartSet)));
}