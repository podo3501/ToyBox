#include "pch.h"
#include "TransformContainer.h"
#include "TransformComponent.h"
#include "JsonOperation.h"
#include "../InputManager.h"
#include "UIType.h"
#include "UIComponent.h"
#include "../Utility.h"

TransformContainer::TransformContainer() :
	m_layout{ XMUINT2{}, Origin::LeftTop }
{
}
TransformContainer::TransformContainer(UIComponent* component, const string& name, const UILayout& layout) :
	m_component{ component },
	m_name{ name },
	m_layout{ layout }
{
}

TransformContainer& TransformContainer::operator=(const TransformContainer& other)
{
	m_name = other.m_name;
	m_layout = other.m_layout;
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
	m_components{ move(o.m_components) },
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) }
{
}

TransformContainer& TransformContainer::operator=(TransformContainer&& o) noexcept
{
	if (this == &o) return *this;

	m_name = move(o.m_name);
	m_layout = move(o.m_layout);
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
	ReturnIfFalse(tie(m_name, m_layout, m_components) == tie(o.m_name, o.m_layout, o.m_components));
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
	for (const auto& child : component->GetTransformContainer().m_components)
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
		auto& currTransformContainer = transform.GetTransformContainer();
		currTransformContainer.SetParent(parent);
	}
}

Rectangle TransformContainer::GetTotalChildSize(const TransformContainer* transformContainer) const noexcept
{
	const XMINT2& curPosition = GetPosition();
	const XMUINT2& curSize = m_layout.GetSize();
	Rectangle rect{ curPosition.x, curPosition.y, static_cast<long>(curSize.x), static_cast<long>(curSize.y) };

	for (const auto& transCom : transformContainer->m_components)
		rect = Rectangle::Union(rect, GetTotalChildSize(&transCom.GetTransformContainer()));

	return rect;
}

void TransformContainer::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
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
	transformComponent->SetRelativePosition(m_parent->m_layout.GetSize(), relativePos);
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

void TransformContainer::GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept
{
	if (IsArea(pos))
		outList.push_back(m_component);

	ForEachComponent([this, &pos, &outList](auto& transCom) {
		const auto& curPosition = pos - m_layout.GetPosition(transCom.GetRelativePosition());
		transCom.component->GetTransformContainer().GetComponents(curPosition, outList);
		});
}

XMINT2 TransformContainer::GetPosition() const noexcept
{
	XMINT2 parentPosition = GetParentPosition();
	return GetPositionByLayout(parentPosition);
}

XMINT2 TransformContainer::GetPositionByLayout(const XMINT2& position) const noexcept
{
	return position + m_layout.GetPosition();
}

void TransformContainer::GenerateUniqueName(TransformContainer* addable) noexcept
{
	//붙는 쪽도 붙여지는 쪽도 유니크 이름이어야 한다.
	auto isNotUnique = [this, addable](const string& name) {
		return !(IsUniqueName(name, addable) &&
			addable->IsUniqueName(name, addable));
		};

	int n = 0;
	string baseName = EnumToString(addable->m_component->GetTypeID()) + "_";
	string curName{};
	do {
		curName = baseName + to_string(n++);
	} while (isNotUnique(curName));
	addable->Rename(curName);

	for (auto& transCom : addable->GetTransformComponents())
		GenerateUniqueName(&transCom.component->GetTransformContainer());
}

XMUINT2 TransformContainer::GetTotalChildSize() const noexcept
{
	const Rectangle& totalRectangle = GetTotalChildSize(this);
	return {
		static_cast<uint32_t>(totalRectangle.width),
		static_cast<uint32_t>(totalRectangle.height) };
}

void TransformContainer::ChangeSize(const XMUINT2& size) noexcept
{
	AdjustChildPosition(size);
	ApplySize(size);
}

bool TransformContainer::ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	ReturnIfFalse(ChangeChildPosition(index, size, relativePos));
	MarkDirty();

	return true;
}