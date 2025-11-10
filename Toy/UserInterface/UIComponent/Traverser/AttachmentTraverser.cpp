#include "pch.h"
#include "AttachmentTraverser.h"
#include "../UIComponent.h"
#include "UserInterface/UIComponentLocator.h"

using namespace UIManager;

unique_ptr<UIComponent> AttachmentTraverser::AttachComponent(UIComponent* parent,
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	if (!parent->HasStateFlag(StateFlag::Attach))
		return move(child);

	UINameGenerator* nameGen = GetNameGenerator();
	bool success = ForEachChildWithRegion(child.get(), GetMyRegion(parent),
		[nameGen](const string& region, UIComponent* component, bool isNewRegion) {
			const string& name = component->GetName().empty() ? EnumToString<ComponentID>(component->GetTypeID()) : component->GetName();
			auto namesOpt = nameGen->MakeNameOf(region, name, isNewRegion);
			if (!namesOpt) return false;
			const auto& [newRegion, newName] = *namesOpt;

			if (isNewRegion) component->SetRegion(newRegion);
			component->SetName(newName);
			return true;
		});

	if (!success)
		return move(child);

	return parent->AttachComponent(move(child), relativePos);
}