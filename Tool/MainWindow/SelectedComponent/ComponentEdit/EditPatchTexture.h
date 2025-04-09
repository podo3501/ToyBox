#pragma once
#include "EditWindow.h"

class EditCombo;
class TextureResourceBinder;
class PatchTexture1;
class EditPatchTexture1 : public EditWindow
{
public:
	~EditPatchTexture1();
	EditPatchTexture1() = delete;
	EditPatchTexture1(PatchTexture1* patchTex1, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	PatchTexture1* m_patchTex1;
	unique_ptr<EditCombo> m_combo;
};

class PatchTexture3;
class EditPatchTexture3 : public EditWindow
{
public:
	~EditPatchTexture3();
	EditPatchTexture3() = delete;
	EditPatchTexture3(PatchTexture3* patchTex3, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	PatchTexture3* m_patchTex3;
	unique_ptr<EditCombo> m_combo;
};

class PatchTexture9;
class EditPatchTexture9 : public EditWindow
{
public:
	~EditPatchTexture9();
	EditPatchTexture9() = delete;
	EditPatchTexture9(PatchTexture9* patchTex9, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept;

protected:
	virtual void RenderComponent() override;

private:
	PatchTexture9* m_patchTex9;
	unique_ptr<EditCombo> m_combo;
};
