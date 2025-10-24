#include "pch.h"
#include "PatchTextureStd1T.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

namespace UserInterface::UIComponentT::ComponentT
{
	TEST_F(PatchTextureStd1T, Create)
	{
		auto patchTex = CreateComponent<PatchTextureStd1>("MockImage");
		//patchTex->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController());
	}
}