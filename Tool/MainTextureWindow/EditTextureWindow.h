#pragma once
#include "../Toy/UserInterface/TextureSourceBinder.h"

enum class ImagePart;
enum class PendingAction;
struct IRenderer;
class ImageGrid1;
struct TextureSourceInfo;
class TextureSourceBinder;
class MainTextureWindow;
class RenameNotifier;

class EditTextureWindow
{
public:
    ~EditTextureWindow();
    EditTextureWindow(IRenderer* renderer);

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

    void RenderTextureInfo();
    void RenderLabeledAreas() const;
    void RenderHighlightArea() const;
  
    Rectangle FindAreaFromMousePos(const XMINT2& pos) const noexcept;

    MainTextureWindow* m_textureWindow;
    TextureSourceBinder* m_sourceBinder;
    int m_preTextureIdx{ -1 };
    vector<Rectangle> m_areaList;
    vector<Rectangle> m_hoveredAreas{};

    int m_sourceIdx{ 0 };
    unique_ptr<RenameNotifier> m_renameNotifier;
    optional<TextureSourceInfo> m_sourceAreas;

    ////////////////////////////////////////

    inline bool IsVaildTextureIndex() const noexcept { return m_textureIdx >= 0 && m_textureIdx < m_textureFiless.size(); }
    void AddTextureFile(const wstring& filename) noexcept;
    void AddTexture(unique_ptr<ImageGrid1> texture) noexcept;
    void RemoveTextureFile(const wstring& filename) noexcept;
    wstring SelectedTextureFilename() const noexcept;
    bool IsLoadedTexture(const wstring& filename) const noexcept;
    bool LoadTextureFile();
    bool DeleteTextureFile() noexcept;
    bool SelectTextureFile() noexcept;
    bool ExecuteAction() noexcept;
    void RenderTextureList();
    void DrawImagePart();
    
    IRenderer* m_renderer;
    optional<PendingAction> m_pendingAction;
    int m_textureIdx{ -1 };
    vector<string> m_textureFiles; //del
    vector<unique_ptr<ImageGrid1>> m_textureFiless;
    ImagePart m_selectImagePart;

};