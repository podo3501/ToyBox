#include "pch.h"
#include "DerivedTraverser.h"
#include "../UIComponent.h"

void DerivedTraverser::Render(UIComponent* c, ITextureRender* render)
{
	//9방향 이미지는 같은 레벨인데 9방향 이미지 위에 다른 이미지를 올렸을 경우 BFS가 아니면 밑에 이미지가 올라온다.
	//가장 밑에 레벨이 가장 위에 올라오는데 DFS(Depth First Search)이면 가장 밑에 있는게 가장 나중에 그려지지 않게 된다.
	ForEachChildToRender(c, [render](UIComponent* component) {
		component->ImplementRender(render);

		return (component->GetTypeID() != ComponentID::RenderTexture)
			? TraverseResult::Continue
			: TraverseResult::ChildrenSkip; //RenderTexture이면 자식들은 랜더하지 않는다. RenderTexture에 랜더링 됐기 때문에.
		});
}

bool DerivedTraverser::BindTextureSourceInfo(UIComponent* c, TextureResourceBinder* resBinder, ITextureController* texController) noexcept
{
	auto forEachResult = ForEachChildPostUntilFail(c, [resBinder, texController](UIComponent* component) {
		bool result = component->ImplementBindSourceInfo(resBinder, texController);
		AssertMsg(result, "Failed to load texture");
		return result;
		});
	ReturnIfFalse(forEachResult);
	ReturnIfFalse(c->UpdatePositionsManually());
	return true;
}

void DerivedTraverser::PropagateRoot(UIComponent* c, UIComponent* root) noexcept
{
	ForEachChild(c, [root](UIComponent* component) { 
		component->m_root = root; 
		});
}

bool DerivedTraverser::EnableToolMode(UIComponent* c, bool enable) noexcept
{
	auto modeFunc = enable ? &UIComponent::EnterToolMode : &UIComponent::ExitToolMode;
	return ForEachChildPostUntilFail(c, [enable, modeFunc](UIComponent* component) {
		if (component->m_toolMode == enable) return true;

		component->m_toolMode = enable;
		ReturnIfFalse((component->*modeFunc)());
		return true;
		});
}