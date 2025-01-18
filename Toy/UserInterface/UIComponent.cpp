#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "JsonOperation.h"
#include "UIComponentHelper.h"

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_layout{ XMUINT2{}, Origin::LeftTop }
{}

UIComponent::UIComponent(const string& name, const UILayout& layout) noexcept :
	m_name{ name },
	m_layout{ layout }
{}

//상속받은 곳에서만 복사생성자를 호출할 수 있다.
UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_layout = other.m_layout;
	ranges::transform(other.m_components, back_inserter(m_components), [this](const auto& transCom) {
		auto component = transCom.component->Clone();
		component->SetParent(this);
		return move(TransformComponent(
			move(component), 
			transCom.GetRelativePosition(), 
			transCom.GetRatio()));
		});
	m_enable = other.m_enable;
}

string UIComponent::GetType() const { return string(typeid(*this).name()); }

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetType() != o.GetType()) return false;

	ReturnIfFalse(tie(m_name, m_layout, m_components) == 
		tie(o.m_name, o.m_layout, o.m_components));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));

	return true;
}

bool UIComponent::EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept
{
	if (lhs == nullptr && rhs == nullptr) return true;
	if (lhs == nullptr || rhs == nullptr) return false;

	if (lhs->m_name != rhs->m_name) return false;

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_components{ move(o.m_components) }
{}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	auto clone = CreateClone();
	clone->m_name = clone->m_name + "_clone";
	return clone;
}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& transComp) {
		return transComp.component->LoadResources(load);
		});
}

bool UIComponent::SetDatas(IGetValue* value)
{
	return ranges::all_of(m_components, [value](const auto& transCom) {
		return transCom.component->SetDatas(value);
		});
}

bool UIComponent::RefreshPosition() noexcept
{
	UIComponent* component = GetRoot();

	return component->RefreshPosition({});
}

bool UIComponent::RefreshPosition(const XMINT2& position) noexcept
{
	if (!m_enable) return true;

	ReturnIfFalse(ImplementUpdate(position));

	auto result = ranges::all_of(m_components, [this, &position](auto& transCom) {
		const auto& curPosition = transCom.GetPosition(m_isDirty, m_layout, position);
		auto updateResult = transCom.component->RefreshPosition(curPosition);
		return updateResult;
		});
	m_isDirty = false;

	return result;
}

bool UIComponent::ProcessUpdate(const XMINT2& position, const InputManager& inputManager) noexcept
{
	if (!m_enable) return true;

	ReturnIfFalse(ImplementUpdate(position));
	ReturnIfFalse(ImplementInput(inputManager));

	auto result = ranges::all_of(m_components, [this, &position, &inputManager](auto& transCom) {
		const auto& curPosition = transCom.GetPosition(m_isDirty, m_layout, position);
		MouseTracker& curMouse = const_cast<InputManager&>(inputManager).GetMouse();
		curMouse.PushOffset(curPosition);
		auto updateResult = transCom.component->ProcessUpdate(curPosition, inputManager);
		curMouse.PopOffset();
		return updateResult;
		});
	m_isDirty = false;

	return result;
}

void UIComponent::ProcessRender(IRender* render)
{
	//9방향 이미지는 같은 레벨인데 9방향 이미지 위에 다른 이미지를 올렸을 경우 BFS가 아니면 밑에 이미지가 올라온다.
	queue<UIComponent*> q;
	q.push(this);

	while (!q.empty())
	{
		UIComponent* current = q.front();
		q.pop();
		
		if (!current->m_enable) continue;

		current->ImplementRender(render);

		for (const auto& child : current->m_components)
			q.push(child.component.get());  
	}
}

//크기를 바꾸면 이 컴포넌트의 자식들의 위치값도 바꿔준다.
void UIComponent::ChangeSize(const XMUINT2& size) noexcept 
{ 
	ranges::for_each(m_components, [&size](auto& transCom) {
		transCom.AdjustPosition(size);
		});

	ApplySize(size);
}

bool UIComponent::ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	if (index >= m_components.size()) return false;
	m_components[index].SetRelativePosition(size, relativePos);
	MarkDirty();

	return true;
}

void UIComponent::GenerateUniqueName(UIComponent* addable) noexcept
{
	int n = 0;
	string baseName = std::regex_replace(addable->GetType(), std::regex(R"(class|\s)"), "") + "_";
	string curName{};
	do {
		curName = baseName + to_string(n++);
	} while (!IsUniqueName(curName) ||
		!addable->IsUniqueName(curName));
	addable->Rename(curName);

	ranges::for_each(addable->m_components, [this](auto& transCom) {
		GenerateUniqueName(transCom.component.get());
		});
}

unique_ptr<UIComponent> UIComponent::AttachComponent(unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept
{
	if (!IsAttachable()) return component;

	GenerateUniqueName(component.get());
	component->SetParent(this);
	auto transComponent = TransformComponent(move(component), GetSize(), relativePos);
	m_components.emplace_back(move(transComponent));
	MarkDirty();

	return nullptr;
}

optional<unique_ptr<UIComponent>> UIComponent::DetachComponent(UIComponent* detachComponent) noexcept
{
	auto find = ranges::find_if(m_components, [detachComponent](auto& transComponent) {
		return (transComponent.component.get() == detachComponent);
		});
	if (find == m_components.end()) return nullopt;

	unique_ptr<UIComponent> detachedComponent = move(find->component);
	m_components.erase(find);
	
	detachedComponent->m_parent = nullptr;
	detachedComponent->MarkDirty();
	detachedComponent->RefreshPosition();

	return detachedComponent;
}

optional<unique_ptr<UIComponent>> UIComponent::DetachComponent() noexcept
{
	if (!IsDetachable()) return nullopt;
	if (!m_parent) return nullopt;

	return move(m_parent->DetachComponent(this));
}

bool UIComponent::IsUniqueName(const string& name) noexcept
{
	UIComponent* findComponent = GetRoot()->GetComponent(name);
	if (findComponent) return false;

	return true;
}

bool UIComponent::Rename(const string& name) noexcept
{
	ReturnIfFalse(IsUniqueName(name));
	m_name = name;

	return true;
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
	operation.Process("Components", m_components);
	
	if (operation.IsWrite()) return;
	ranges::for_each(m_components, [this](auto& transComponent) {
		transComponent.component->SetParent(this);
		});
}

void UIComponent::MarkDirty() noexcept
{
	if (m_isDirty) return;
	
	m_isDirty = true;
	ranges::for_each(m_components, [](auto& transComponent) {
		transComponent.component->MarkDirty();
		});
}

XMUINT2 UIComponent::GetTotalChildSize() const noexcept
{
	const Rectangle& totalRectangle = GetTotalChildSize(this);
	return { 
		static_cast<uint32_t>(totalRectangle.width), 
		static_cast<uint32_t>(totalRectangle.height) };
}

Rectangle UIComponent::GetTotalChildSize(const UIComponent* component) const noexcept
{
	if (component == nullptr) return {};

	Rectangle rect = GetRectangle(component);

	//단순한 크기가 아니라 위치값이 들어가는 사각형의 크기가 필요하다.
	for (const auto& transCom : component->m_components)
		rect = Rectangle::Union(rect, GetTotalChildSize(transCom.component.get()));

	return rect;
}

bool UIComponent::GetRelativePosition(XMINT2& outRelativePos) const noexcept
{
	if (!m_parent) return false;
	
	auto transformComponent = m_parent->FindTransformComponent(this);
	outRelativePos = transformComponent->GetRelativePosition();

	return true;
}

bool UIComponent::SetRelativePosition(const XMINT2& relativePos) noexcept
{
	if (!m_parent) return false;

	auto transformComponent = m_parent->FindTransformComponent(this);
	transformComponent->SetRelativePosition(m_parent->GetSize(), relativePos);
	m_parent->MarkDirty();

	return true;
}

XMINT2 UIComponent::GetPosition() const noexcept
{
	XMINT2 parentPosition{ 0, 0 };
	if (m_parent)
	{
		auto transformComponent = m_parent->FindTransformComponent(this);
		parentPosition = transformComponent->absolutePosition;
	}

	return GetPositionByLayout(parentPosition);
}

XMINT2 UIComponent::GetPositionByLayout(const XMINT2& position) const noexcept
{
	return position + m_layout.GetPosition();
}

UIComponent* UIComponent::GetRoot() noexcept
{
	UIComponent* current = this;
	while (current->m_parent != nullptr)
		current = current->m_parent;
	
	return current;
}

void UIComponent::GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept
{
	if (IsArea(pos))
		outList.push_back(this);

	ranges::for_each(m_components, [this, &pos, &outList](auto& transCom) {
		const auto& curPosition = pos - m_layout.GetPosition(transCom.GetRelativePosition());
		transCom.component->GetComponents(curPosition, outList);
		});
}

UIComponent* UIComponent::GetComponent(const string& name) const noexcept
{
	if (m_name == name)
		return const_cast<UIComponent*>(this);

	for (const auto& transComponent : m_components)
	{
		UIComponent* find = transComponent.component->GetComponent(name);
		if (find) return find;
	}
	
	return nullptr;
}

vector<UIComponent*> UIComponent::GetComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_components, back_inserter(componentList), [](const auto& transCom) {
		return transCom.component.get();
		});

	return componentList;
}

template<typename Predicate>
TransformComponent* FindTransformComponentIf(auto& components, Predicate&& pred) noexcept
{
	auto find = ranges::find_if(components, std::forward<Predicate>(pred));
	return (find != components.end()) ? &(*find) : nullptr;
}

TransformComponent* UIComponent::FindTransformComponent(const std::string& name) noexcept
{
	return FindTransformComponentIf(m_components, [&name](const auto& transComp) {
		return transComp.component->GetName() == name;
		});
}

TransformComponent* UIComponent::FindTransformComponent(const UIComponent* component) noexcept
{
	return FindTransformComponentIf(m_components, [component](const auto& transComp) {
		return transComp.component.get() == component;
		});
}



