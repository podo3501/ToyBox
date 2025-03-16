#pragma once
#include "../Toy/UserInterface/TextureSourceBinder.h"

enum class ImagePart : int;
struct TextureSourceInfo;
class TextureSourceBinder;
class MainTextureWindow;
class RenameNotifier;

class EditTextureWindow
{
public:
    ~EditTextureWindow();
    EditTextureWindow();

    void SetResourceAreas(optional<TextureSourceInfo> resourceAreas) noexcept;
    void Update() noexcept;
    void Render();

    void SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept;
    inline void SetTextureWindow(MainTextureWindow* texWindow) noexcept { m_textureWindow = texWindow; }
    inline void SetTextureAreaList(const vector<Rectangle>& areaList) noexcept { m_areaList = areaList; }

private:
    void CheckSourcePartition();
    bool DeselectImageSource() noexcept;
    void SelectImageSource() noexcept;

    void RenderTextureList();
    void RenderTextureInfo();
    void RenderHighlightArea() const;
    void DrawImagePart();
    Rectangle FindAreaFromMousePos() const noexcept;
    wstring SelectedTextureFilename() const noexcept;

    MainTextureWindow* m_textureWindow;
    TextureSourceBinder* m_sourceBinder;
    int m_preTextureIdx{ -1 };
    int m_textureIdx{ -1 };
    vector<Rectangle> m_areaList;
    vector<Rectangle> m_hoveredAreas{};
    vector<string> m_textureFiles;
    int m_sourceIdx{ 0 };
    ImagePart m_selectImagePart;
    unique_ptr<RenameNotifier> m_renameNotifier;
    optional<TextureSourceInfo> m_sourceAreas;
};