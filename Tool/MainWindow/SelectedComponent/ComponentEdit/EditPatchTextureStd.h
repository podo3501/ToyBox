#pragma once
#include "EditWindow.h"

class EditCombo;
class TextureResourceBinder;
class PatchTextureStd;
class EditPatchTextureStd : public EditWindow
{
public:
	~EditPatchTextureStd();
	EditPatchTextureStd() = delete;
	EditPatchTextureStd(PatchTextureStd* patchTex, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void SetupComponent() noexcept;
	virtual void RenderComponent() override;

	unique_ptr<EditCombo> m_combo;
	PatchTextureStd* m_patchTex;
};

////////////////////////////////////////////////

class PatchTextureStd1;
class EditPatchTextureStd1 : public EditPatchTextureStd
{
public:
	~EditPatchTextureStd1();
	EditPatchTextureStd1() = delete;
	EditPatchTextureStd1(PatchTextureStd1* patchTex1, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;
};

class PatchTextureStd3;
class EditPatchTextureStd3 : public EditPatchTextureStd
{
public:
	~EditPatchTextureStd3();
	EditPatchTextureStd3() = delete;
	EditPatchTextureStd3(PatchTextureStd3* patchTex3, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	PatchTextureStd3* m_patchTex3;
};

class PatchTextureStd9;
class EditPatchTextureStd9 : public EditPatchTextureStd
{
public:
	~EditPatchTextureStd9();
	EditPatchTextureStd9() = delete;
	EditPatchTextureStd9(PatchTextureStd9* patchTex9, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

private:
	PatchTextureStd9* m_patchTex9;
};
