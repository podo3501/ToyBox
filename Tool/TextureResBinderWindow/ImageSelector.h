#pragma once

enum class TextureSlice;
class TextureResBinderWindow;
class TexResCommandList;
struct TextureSourceInfo;
class PatchTexture1;
class RenameNotifier;
class ImageSelector
{
public:
	~ImageSelector();
	ImageSelector(TextureResBinderWindow* textureWindow);

	void Update();
	void Render();
	void SetTexture(PatchTexture1* pTex1) noexcept;
	void SetCommandList(TexResCommandList* cmdList) noexcept { m_cmdList = cmdList; }
	bool DeselectArea() noexcept;

private:
	void CheckSourcePartition() noexcept;
	void CheckSelectArea() noexcept;
	void SelectArea() noexcept;	
	bool RemoveArea() noexcept;
	optional<Rectangle> FindAreaFromMousePos(const XMINT2& pos) const noexcept;
	void SelectImagePart();
	void EditSelectArea();
	void RenderHoveredArea() const;
	void RenderSelectedArea() const;
	void RenderLabeledAreas() const;
	

	PatchTexture1* m_sourceTexture;
	TexResCommandList* m_cmdList;
	TextureResBinderWindow* m_textureWindow;
	unique_ptr<RenameNotifier> m_renameNotifier;
	TextureSlice m_selectImagePart;
	vector<Rectangle> m_areaList; //�ڵ����� ã�Ƴ��� ������
	vector<Rectangle> m_hoveredAreas{}; //���콺�� ����������� ������
	unique_ptr<TextureSourceInfo> m_selectedArea;
};