#pragma once
#include "EditWindow.h"

class EditCombo;
class TextureResourceBinder;
class PatchTexture;
class EditPatchTexture : public EditWindow
{
public:
	~EditPatchTexture();
	EditPatchTexture() = delete;
	EditPatchTexture(PatchTexture* patchTex, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void SetupComponent() noexcept;
	virtual void RenderComponent() override;

	unique_ptr<EditCombo> m_combo;
	PatchTexture* m_patchTex;
};

////////////////////////////////////////////////

class PatchTexture1;
class EditPatchTexture1 : public EditPatchTexture
{
public:
	~EditPatchTexture1();
	EditPatchTexture1() = delete;
	EditPatchTexture1(PatchTexture1* patchTex1, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;
};

class PatchTexture3;
class EditPatchTexture3 : public EditPatchTexture
{
public:
	~EditPatchTexture3();
	EditPatchTexture3() = delete;
	EditPatchTexture3(PatchTexture3* patchTex3, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	PatchTexture3* m_patchTex3;
};

class PatchTexture9;
class EditPatchTexture9 : public EditPatchTexture
{
public:
	~EditPatchTexture9();
	EditPatchTexture9() = delete;
	EditPatchTexture9(PatchTexture9* patchTex9, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	PatchTexture9* m_patchTex9;
};
