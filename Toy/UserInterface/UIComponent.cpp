#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "JsonOperation.h"

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_transformContainer{ this, "", {}}
{}

UIComponent::UIComponent(const string& name, const UILayout& layout) noexcept :
	m_name{ name },
	m_transformContainer{ this, name, layout }
{}

//상속받은 곳에서만 복사생성자를 호출할 수 있다.
UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_enable = other.m_enable;
	m_attachmentState = other.m_attachmentState;
	m_transformContainer = other.m_transformContainer;
	m_transformContainer.SetComponent(this);
}

//string UIComponent::GetType() const { return string(typeid(*this).name()); }

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetTypeID() != o.GetTypeID()) return false;

	ReturnIfFalse(tie(m_name, m_enable, m_attachmentState, m_transformContainer) ==
		tie(o.m_name, o.m_enable, o.m_attachmentState, o.m_transformContainer));

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_enable{ move(o.m_enable) },
	m_attachmentState{ move(o.m_attachmentState) },
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
		const auto& curPosition = transCom.GetPosition(m_transformContainer.GetDirty(), m_transformContainer.m_layout, position);
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
	return m_transformContainer.Update(inputManager, m_transformContainer.m_layout, position);
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
	m_transformContainer.ChangeSize(size);
}

unique_ptr<UIComponent> UIComponent::AttachComponent(unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept
{
	if (!IsAttachable()) return component;

	component->m_parent = this;
	m_children.push_back(component.get());

	m_transformContainer.GenerateUniqueName(&component->GetTransformContainer());
	component->m_transformContainer.SetParent(&m_transformContainer);
	m_transformContainer.AttachChildComponent(move(component), m_transformContainer.m_layout.GetSize(), relativePos);
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

	detached->m_parent = nullptr;
	erase(m_children, detached.get());

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
	operation.Process("Name", m_name);
	operation.Process("Enable", m_enable);
	operation.Process("AttachmentState", m_attachmentState);
	operation.Process("TransformContainer", m_transformContainer);
}

UIComponent* UIComponent::FindRoot()
{
	if (!m_parent)
		return this;

	return m_parent->FindRoot();
}

void UIComponent::ForEachChild(std::function<void(UIComponent*)> func) noexcept
{
	func(this);

	for (UIComponent* child : m_children) {
		if (child) {
			child->ForEachChild(func);
		}
	}
}

//////////////////////////////////////////////////////////////////

UIComponent* GetComponentNew(UIComponent* component, const string& targetName)
{
	// 루트를 찾기
	UIComponent* root = component->FindRoot();
	UIComponent* foundComponent = nullptr;

	// Traverse를 사용하여 트리 구조에서 이름을 찾기
	root->ForEachChild([&foundComponent, &targetName](UIComponent* comp) {
		if (comp->GetName() == targetName) {
			foundComponent = comp;
		}
		});

	return foundComponent;
}