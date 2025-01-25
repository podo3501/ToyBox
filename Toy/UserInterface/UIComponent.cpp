#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "JsonOperation.h"
#include "UIComponentEx.h"

TransformContainer::TransformContainer()
{
}
TransformContainer::TransformContainer(UIComponent* component, const string& name) :
	m_component{ component },
	m_name{ name } {}
TransformContainer& TransformContainer::operator=(const TransformContainer& other)
{
	m_name = other.m_name;
	ranges::transform(other.m_components, back_inserter(m_components), [this](const auto& transCom) {
		auto component = transCom.component->Clone();
		return move(TransformComponent(
			move(component),
			transCom.GetRelativePosition(),
			transCom.GetRatio()));
		});
	SetParents(this);

	return *this;
}

TransformContainer::TransformContainer(TransformContainer&& o) noexcept :
	m_name{ move(o.m_name) },
	m_components{ move(o.m_components) }
{}

TransformContainer& TransformContainer::operator=(TransformContainer&& o) noexcept
{
	if (this == &o) return *this;

	m_name = move(o.m_name);
	m_components = move(o.m_components);

	return *this;
}

bool TransformContainer::EqualComponent(const TransformContainer* lhs, const TransformContainer* rhs) const noexcept
{
	if (lhs == nullptr && rhs == nullptr) return true;
	if (lhs == nullptr || rhs == nullptr) return false;
	if (lhs->GetName() != rhs->GetName()) return false;

	return true;
}

bool TransformContainer::operator==(const TransformContainer& o) const noexcept
{
	ReturnIfFalse(tie(m_name, m_components) == tie(m_name, o.m_components));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));
	return true;
}

bool TransformContainer::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& transComp) {
		return transComp.component->LoadResources(load);
			});
}

bool TransformContainer::SetDatas(IGetValue* value)
{
	return ranges::all_of(m_components, [value](const auto& transCom) {
		return transCom.component->SetDatas(value);
		});
}

bool TransformContainer::Update(const InputManager& inputManager,
	const UILayout& layout, const XMINT2& position)
{
	bool result = ranges::all_of(m_components, [this, &inputManager, &layout, &position](auto& transCom) {
		const auto& curPosition = transCom.GetPosition(m_isDirty, layout, position);
		MouseTracker& curMouse = const_cast<InputManager&>(inputManager).GetMouse();
		curMouse.PushOffset(curPosition);
		auto updateResult = transCom.component->ProcessUpdate(curPosition, inputManager);
		curMouse.PopOffset();
		return updateResult;
		});
	m_isDirty = false;
	return result;
}

bool TransformContainer::IsUniqueName(const string& name, TransformContainer* self) noexcept
{
	TransformContainer* find = GetRoot()->GetComponent(name);
	if (find && find != self) return false;

	return true;
}

void TransformContainer::PushChildComponents(UIComponent* component, queue<UIComponent*>& outQueue)
{
	for (const auto& child : component->m_transformContainer.m_components)
		outQueue.push(child.component.get());
}

void TransformContainer::AdjustChildPosition(const XMUINT2& size)
{
	ranges::for_each(m_components, [&size](auto& transCom) {
		transCom.AdjustPosition(size);
		});
}

bool TransformContainer::ChangeChildPosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	if (index >= m_components.size()) return false;
	m_components[index].SetRelativePosition(size, relativePos);

	return true;
}

void TransformContainer::AttachChildComponent(
	unique_ptr<UIComponent> component, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	auto transComponent = TransformComponent(move(component), size, relativePos);
	m_components.emplace_back(move(transComponent));
}

unique_ptr<UIComponent> TransformContainer::DetachChildComponent(UIComponent* detach) noexcept
{
	auto find = ranges::find_if(m_components, [detach](auto& transComponent) {
		return (transComponent.component.get() == detach);
		});
	if (find == m_components.end()) return nullptr;

	unique_ptr<UIComponent> detachedComponent = move(find->component);
	m_components.erase(find);

	return detachedComponent;
}

void TransformContainer::SetParents(TransformContainer* parent) noexcept
{
	for (auto& transform : m_components)
	{
		auto& currTransformContainer = transform.component->GetTransformContainer();
		currTransformContainer.SetParent(parent);
	}
}

Rectangle TransformContainer::GetTotalChildSize(const UIComponent* component) const noexcept
{
	if (component == nullptr) return {};

	Rectangle rect = UIComponentEx::GetRectangle(component);
	for (const auto& transCom : component->m_transformContainer.m_components)
		rect = Rectangle::Union(rect, GetTotalChildSize(transCom.component.get()));

	return rect;
}

void TransformContainer::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Components", m_components);

	if (operation.IsWrite()) return;
	SetParents(this);
}

void TransformContainer::ForEachComponent(const std::function<void(TransformComponent&)>& action) noexcept
{
	ranges::for_each(m_components, action);
}

bool TransformContainer::AllOfComponent(const std::function<bool(TransformComponent&)>& action) noexcept
{
	return ranges::all_of(m_components, [&](TransformComponent& transComponent) {
		return action(transComponent);
		});
}

UIComponent* TransformContainer::FindIfComponent(const std::function<UIComponent* (TransformComponent&)>& action) noexcept
{
	auto it = ranges::find_if(m_components, [&](TransformComponent& transComponent) {
		return action(transComponent) != nullptr;
		});

	return (it != m_components.end()) ? it->component.get() : nullptr;
}

TransformComponent* TransformContainer::FindIfTransformComponent(
	const function<bool(const TransformComponent&)>& action) noexcept
{
	auto it = ranges::find_if(m_components, [&](TransformComponent& transComponent) {
		return action(transComponent);
		});

	return (it != m_components.end()) ? &(*it) : nullptr;
}

vector<UIComponent*> TransformContainer::GetComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_components, back_inserter(componentList), [](const auto& transCom) {
		return transCom.component.get();
		});

	return componentList;
}

TransformContainer* TransformContainer::GetRoot() noexcept
{
	TransformContainer* current = this;
	while (current->m_parent != nullptr)
		current = current->m_parent;

	return current;
}


TransformContainer* TransformContainer::GetComponent(const string& name) noexcept
{
	if (m_name == name)
		return const_cast<TransformContainer*>(this);

	auto it = ranges::find_if(m_components, [&](const auto& transCom) {
		return transCom.component->GetTransformContainer().GetComponent(name) != nullptr;
		});

	if (it != m_components.end()) //찾았는데 다시 돌아가서 리턴값을 만들어주는 부분이군. 이상하다.
		return it->component->GetTransformContainer().GetComponent(name);

	return nullptr;
}

bool TransformContainer::Rename(const string& name) noexcept
{
	if (m_name == name) return true;

	ReturnIfFalse(IsUniqueName(name, this));
	m_name = name;

	return true;
}

TransformComponent* TransformContainer::FindIfTransformComponent(const std::string& name) noexcept
{
	return FindIfTransformComponent([&name](const TransformComponent& transCom) {
		return transCom.component->GetTransformContainer().GetName() == name;
		});
}

TransformComponent* TransformContainer::FindIfTransformComponent(const UIComponent* component) noexcept
{
	return FindIfTransformComponent([component](const auto& transCom) {
		return transCom.component.get() == component;
		});
}

optional<XMINT2> TransformContainer::GetRelativePosition() const noexcept
{
	if (!m_parent) return nullopt;

	auto transformComponent = m_parent->FindIfTransformComponent(m_component);
	return transformComponent->GetRelativePosition();
}

bool TransformContainer::SetRelativePosition(const XMINT2& relativePos) noexcept
{
	if (!m_parent) return false;

	auto transformComponent = m_parent->FindIfTransformComponent(m_component);
	transformComponent->SetRelativePosition(UIComponentEx::GetSize(m_parent->m_component), relativePos);
	m_parent->MarkDirty();

	return true;
}

void TransformContainer::MarkDirty() noexcept
{
	if (m_isDirty) return;

	m_isDirty = true;
	ForEachComponent([](auto& transCom) { transCom.component->GetTransformContainer().MarkDirty(); });
}

XMINT2 TransformContainer::GetParentPosition() const noexcept
{
	if (!m_parent) return {};

	auto transformComponent = m_parent->FindIfTransformComponent(m_component);
	return transformComponent->absolutePosition;
}

bool TransformContainer::IsUniqueName(const string& name, UIComponent* self) noexcept
{
	TransformContainer* findTransformContainer = GetRoot()->GetComponent(name);
	if (findTransformContainer && findTransformContainer->m_component != self) return false;

	return true;
}

////////////////////////////////////////////////////////////////////

using namespace UIComponentEx;

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_layout{ XMUINT2{}, Origin::LeftTop },
	m_transformContainer{ this, "" }
{}

UIComponent::UIComponent(const string& name, const UILayout& layout) noexcept :
	m_transformContainer{ this, name },
	m_layout{ layout }
{}

//상속받은 곳에서만 복사생성자를 호출할 수 있다.
UIComponent::UIComponent(const UIComponent& other)
{
	/*m_name = other.m_name;*/
	m_layout = other.m_layout;
	m_enable = other.m_enable;
	m_attachmentState = other.m_attachmentState;
	m_transformContainer = other.m_transformContainer;
	m_transformContainer.SetComponent(this);
}

//string UIComponent::GetType() const { return string(typeid(*this).name()); }

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetTypeID() != o.GetTypeID()) return false;

	ReturnIfFalse(tie(m_layout, m_enable, m_attachmentState, m_transformContainer) ==
		tie(o.m_layout, o.m_enable, o.m_attachmentState, o.m_transformContainer));

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	/*m_name{ move(o.m_name) },*/
	m_layout{ move(o.m_layout) },
	m_transformContainer{ move(o.m_transformContainer) }
{}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	return CreateClone();
}

bool UIComponent::LoadResources(ILoadData* load) { return m_transformContainer.LoadResources(load); }
bool UIComponent::SetDatas(IGetValue* value) { return m_transformContainer.SetDatas(value); }

bool UIComponent::RefreshPosition() noexcept
{
	UIComponent* component = m_transformContainer.GetRoot()->GetComponent();
	return component->RefreshPosition({});
}

bool UIComponent::RefreshPosition(const XMINT2& position) noexcept
{
	if (!m_enable) return true;

	ReturnIfFalse(ImplementUpdate(position));

	auto result = m_transformContainer.AllOfComponent([this, &position](auto& transCom)->bool {
		const auto& curPosition = transCom.GetPosition(m_transformContainer.GetDirty(), m_layout, position);
		auto updateResult = transCom.component->RefreshPosition(curPosition);
		return updateResult;
		});

	m_transformContainer.SetDirty(false);

	return result;
}

bool UIComponent::ProcessUpdate(const XMINT2& position, const InputManager& inputManager) noexcept
{
	if (!m_enable) return true;

	ReturnIfFalse(ImplementUpdate(position));
	ReturnIfFalse(ImplementInput(inputManager));
	return  m_transformContainer.Update(inputManager, m_layout, position);
}

void UIComponent::ProcessRender(IRender* render)
{
	//9방향 이미지는 같은 레벨인데 9방향 이미지 위에 다른 이미지를 올렸을 경우 BFS가 아니면 밑에 이미지가 올라온다.
	queue<UIComponent*> componentQueue;
	componentQueue.push(this);

	while (!componentQueue.empty())
	{
		UIComponent* current = componentQueue.front();
		componentQueue.pop();
		
		if (!current->m_enable) continue;

		current->ImplementRender(render);

		m_transformContainer.PushChildComponents(current, componentQueue);
	}
}

//크기를 바꾸면 이 컴포넌트의 자식들의 위치값도 바꿔준다.
void UIComponent::ChangeSize(const XMUINT2& size) noexcept 
{ 
	m_transformContainer.AdjustChildPosition(size);
	ApplySize(size);
}

bool UIComponent::ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	ReturnIfFalse(m_transformContainer.ChangeChildPosition(index, size, relativePos));
	m_transformContainer.MarkDirty();

	return true;
}

void UIComponent::GenerateUniqueName(UIComponent* addable) noexcept
{
	//붙는 쪽도 붙여지는 쪽도 유니크 이름이어야 한다.
	auto isNotUnique = [this, addable](const string& name) {
		return !(m_transformContainer.IsUniqueName(name, addable) && 
			addable->GetTransformContainer().IsUniqueName(name, addable));
		};

	int n = 0;
	string baseName = EnumToString(addable->GetTypeID()) + "_";
	string curName{};
	do {
		curName = baseName + to_string(n++);
	} while (isNotUnique(curName));
	addable->GetTransformContainer().Rename(curName);

	for (auto& transCom : addable->m_transformContainer.GetComponents())
		GenerateUniqueName(transCom.component.get());
}

unique_ptr<UIComponent> UIComponent::AttachComponent(unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept
{
	if (!IsAttachable()) return component;

	GenerateUniqueName(component.get());
	component->m_transformContainer.SetParent(&m_transformContainer);
	m_transformContainer.AttachChildComponent(move(component), m_layout.GetSize(), relativePos);
	m_transformContainer.MarkDirty();

	return nullptr;
}

unique_ptr<UIComponent> UIComponent::DetachComponent(UIComponent* detachComponent) noexcept
{
	unique_ptr<UIComponent> detached = m_transformContainer.DetachChildComponent(detachComponent);
	if (detached == nullptr) return nullptr;
	
	detached->m_transformContainer.SetParent(nullptr);
	detached->m_transformContainer.MarkDirty();
	detached->RefreshPosition();

	return detached;
}

pair<unique_ptr<UIComponent>, UIComponent*> UIComponent::DetachComponent() noexcept
{
	if (!IsDetachable()) return {};
	if (!m_transformContainer.GetParent()) return {};

	UIComponent* parent = m_transformContainer.GetParent()->GetComponent(); //DetachComponent를 하면 parent가 nullptr로 셋팅된다.
	return { move(m_transformContainer.GetParent()->GetComponent()->DetachComponent(this)), parent};
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Layout", m_layout);
	operation.Process("TransformContainer", m_transformContainer);
	operation.Process("Enable", m_enable);
	operation.Process("AttachmentState", m_attachmentState);
}

XMUINT2 UIComponent::GetTotalChildSize() const noexcept
{
	const Rectangle& totalRectangle = m_transformContainer.GetTotalChildSize(this);
	return { 
		static_cast<uint32_t>(totalRectangle.width), 
		static_cast<uint32_t>(totalRectangle.height) };
}

XMINT2 UIComponent::GetPosition() const noexcept
{
	XMINT2 parentPosition = m_transformContainer.GetParentPosition();
	return GetPositionByLayout(parentPosition);
}

XMINT2 UIComponent::GetPositionByLayout(const XMINT2& position) const noexcept
{
	return position + m_layout.GetPosition();
}

void UIComponent::GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept
{
	if (IsArea(pos))
		outList.push_back(this);

	m_transformContainer.ForEachComponent([this, &pos, &outList](auto& transCom) {
		const auto& curPosition = pos - m_layout.GetPosition(transCom.GetRelativePosition());
		transCom.component->GetComponents(curPosition, outList);
		});
}

//UIComponent* UIComponent::GetComponent(const string& name) noexcept
//{
//	if (m_name == name)
//		return const_cast<UIComponent*>(this);
//
//	return m_transformContainer.FindIfComponent(
//		[&name](TransformComponent& transComponent) -> UIComponent* {
//			return transComponent.component->GetComponent(name);
//		}
//	);
//
//	return nullptr;
//}

vector<UIComponent*> UIComponent::GetComponents() const noexcept
{
	return m_transformContainer.GetComponents();
}



