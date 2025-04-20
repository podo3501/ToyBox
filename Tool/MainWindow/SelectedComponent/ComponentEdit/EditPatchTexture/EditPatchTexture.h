#pragma once
#include "../EditWindow.h"

class PatchTexture;
class EditPatchTexture : public EditWindow
{
public:
	~EditPatchTexture();
	EditPatchTexture() = delete;
	EditPatchTexture(PatchTexture* patchTex, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

	PatchTexture* m_patchTex;
};