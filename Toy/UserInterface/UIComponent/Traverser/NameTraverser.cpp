#include "pch.h"
#include "NameTraverser.h"
#include "../UIComponent.h"
#include "UserInterface/UIComponentLocator.h"

using namespace UIManager;

NameTraverser::NameTraverser(UINameGenerator* nameGen) :
	m_nameGen{ nameGen }
{}

UIComponent* NameTraverser::FindComponent(UIComponent* c, const string& name) noexcept
{
	UIComponent* root = GetRegionRoot(c);
	//if (!root) root = c->GetRoot(); //Region이 없을 경우 root가 region root가 되고 ""가 region name이다.
	//const string& region = root->GetRegion();
	//UIComponent* foundComponent = nullptr;
	return nullptr;
}

bool NameTraverser::Rename(UIComponent* c, const string& name) noexcept
{
	const auto& region = c->GetMyRegion();
	if (!m_nameGen->IsUnusedName(region, name)) return false;

	ReturnIfFalse(m_nameGen->RemoveName(region, c->GetName()));
	auto namesOpt = m_nameGen->MakeNameOf(region, name);
	if (!namesOpt) return false;
	
	c->SetName(namesOpt->second);
	return true;
}

bool NameTraverser::RenameRegion(UIComponent* c, const string& newRegion) noexcept
{
	const string oldRegion = c->GetRegion();
	if (oldRegion == newRegion)
		return true;

	UIComponent* parentRoot = GetParentRegionRoot(c);
	if (newRegion == "") // region이 "" 라면 기존 region을 삭제 처리한다.
		return RemoveAndMergeRegion(c, parentRoot, oldRegion);

	//새 region 이름이 이미 존재하면 중복 처리
	if (!m_nameGen->IsUnusedRegion(newRegion))
		return false;

	return ReplaceAndMergeRegion(c, parentRoot, oldRegion, newRegion);
}

void NameTraverser::AssignNamesInRegion(UIComponent* component, const string& region) noexcept
{
	ForEachChildInSameRegion(component, [this, &region, component](UIComponent* curComponent) {
		auto namesOpt = m_nameGen->MakeNameOf(region, curComponent->GetName());
		curComponent->SetName(namesOpt->second);
		if (curComponent == component)
			curComponent->SetRegion(namesOpt->first);
		});
}

bool NameTraverser::RemoveAndMergeRegion(UIComponent* c, UIComponent* parentRoot, const string& oldRegion) noexcept
{
	// 기존 region 제거
	ReturnIfFalse(m_nameGen->RemoveRegion(oldRegion));
	c->SetRegion(""); //region을 삭제한다.

	// root 여부에 따라 재정렬 대상 결정
	UIComponent* target = (c == parentRoot) ? c : parentRoot;
	const string& targetRegion = target->GetRegion();

	ReturnIfFalse(m_nameGen->RemoveRegion(targetRegion));
	AssignNamesInRegion(target, targetRegion);
	return true;
}

bool NameTraverser::ReplaceAndMergeRegion(UIComponent* c, UIComponent* parentRoot, 
	const string& oldRegion, const string& newRegion) noexcept
{
	//기존 region을 제거하고 새 region 재할당
	ReturnIfFalse(m_nameGen->RemoveRegion(oldRegion));
	AssignNamesInRegion(c, newRegion);

	//이전에 region이 없었던 경우 상위도 갱신
	if (oldRegion == "" && c != parentRoot)
	{
		string parentRegion = parentRoot->GetRegion();
		ReturnIfFalse(m_nameGen->RemoveRegion(parentRegion));
		AssignNamesInRegion(parentRoot, parentRegion);
	}

	return true;
}