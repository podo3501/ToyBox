#pragma once

enum class ImagePart;
class TextureSourceWindow;
class TextureSourceBinder;
struct TextureSourceInfo;
class ImageGrid1;
class RenameNotifier;
class ImageSelector
{
public:
	~ImageSelector();
	ImageSelector(TextureSourceWindow* textureWindow);

	void Update();
	void Render();
	void SetTexture(ImageGrid1* grid) noexcept;
	void SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept { m_sourceBinder = sourceBinder; }

private:
	void CheckSourcePartition() noexcept;
	void CheckSelectArea() noexcept;
	void SelectArea() noexcept;	
	bool DeselectArea() noexcept;
	bool RemoveArea() noexcept;
	optional<Rectangle> FindAreaFromMousePos(const XMINT2& pos) const noexcept;
	void SelectImagePart();
	void EditSelectArea();
	void RenderHoveredArea() const;
	void RenderSelectedArea() const;
	void RenderLabeledAreas() const;
	

	ImageGrid1* m_sourceTexture;
	TextureSourceBinder* m_sourceBinder;
	TextureSourceWindow* m_textureWindow;
	unique_ptr<RenameNotifier> m_renameNotifier;
	ImagePart m_selectImagePart;
	vector<Rectangle> m_areaList; //자동으로 찾아놓은 구역들
	vector<Rectangle> m_hoveredAreas{}; //마우스를 갖대댔을때의 구역들
	unique_ptr<TextureSourceInfo> m_selectedArea;
};