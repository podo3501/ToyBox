#include "pch.h"
#include "ImageSelector.h"
#include "MainTextureWindow.h"
#include "../MainWindow/EditUtility.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/UserInterface/UIComponent/UIUtility.h"
#include "../Toy/InputManager.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../HelperClass.h"

ImageSelector::~ImageSelector() = default;
ImageSelector::ImageSelector(MainTextureWindow* textureWindow) :
    m_sourceTexture{ nullptr },
    m_sourceBinder{ nullptr },
    m_textureWindow{ textureWindow },
    m_renameNotifier{ make_unique<RenameNotifier>() },
    m_selectImagePart{ ImagePart::One }
{}

void ImageSelector::SetTexture(ImageGrid1* texture) noexcept
{
    m_sourceTexture = texture;
    if (!texture)
    {
        m_areaList = {};
        return;
    }

    if (const auto& areaList = texture->GetTextureAreaList(); areaList)
        m_areaList = *areaList;

    DeselectArea();
}

void ImageSelector::Update()
{
    CheckSourcePartition();
    CheckSelectArea();
}

void ImageSelector::SelectArea() noexcept
{
    if (!m_hoveredAreas.size() || m_hoveredAreas[0] == Rectangle{}) return;
    m_selectedArea = make_unique<TextureSourceInfo>(m_sourceTexture->GetFilename(), m_selectImagePart, m_hoveredAreas);
}

bool ImageSelector::DeselectArea() noexcept
{
    m_selectedArea.reset();
    return true;
}

bool ImageSelector::RemoveArea() noexcept
{
    if (!m_selectedArea) return false;
    const string& bindingKey = m_sourceBinder->GetBindingKey(*m_selectedArea);
    if (bindingKey.empty()) return false;
    m_sourceBinder->RemoveTextureKey(bindingKey);

    return DeselectArea();
}

void ImageSelector::CheckSelectArea() noexcept
{
    if (IsInputAction(MouseButton::Left, KeyState::Pressed)) SelectArea();
    if (IsInputAction(Keyboard::Escape, KeyState::Pressed)) DeselectArea();
    if (IsInputAction(Keyboard::Delete, KeyState::Pressed)) RemoveArea();
}

optional<Rectangle> ImageSelector::FindAreaFromMousePos(const XMINT2& pos) const noexcept
{
    auto it = ranges::find_if(m_areaList, [&pos](const Rectangle& rect) { return Contains(rect, pos); });
    return (it != m_areaList.end()) ? optional{ *it } : nullopt;
}

enum DivideType : int
{
    None = 0,
    X = 1 << 0,
    Y = 1 << 1,
    XY = X | Y
};

static void DivideLengthByThree(const Rectangle& area, DivideType divideType,
    vector<int>& outWidth, vector<int>& outHeight) noexcept
{
    if (area.IsEmpty()) return;
    outWidth = { area.width };
    outHeight = { area.height };

    auto divideByThree = [](int totalSize) -> vector<int> {
        int oneThird = totalSize / 3;
        return { oneThird, totalSize - oneThird * 2, oneThird };
        };

    if (divideType & DivideType::X) outWidth = divideByThree(area.width);
    if (divideType & DivideType::Y) outHeight = divideByThree(area.height);
}

static vector<Rectangle> GenerateSourceAreas(const Rectangle& area, DivideType divideType) noexcept
{
    if (divideType == DivideType::None) return { area };

    vector<int> widths, heights;
    DivideLengthByThree(area, divideType, widths, heights);
    return GetSourcesFromArea(area, widths, heights);
}

static inline DivideType ImagePartToDivideType(ImagePart imgPart) noexcept
{
    return imgPart == ImagePart::One ? DivideType::None :
           imgPart == ImagePart::ThreeH ? DivideType::X :
           imgPart == ImagePart::ThreeV ? DivideType::Y :
           imgPart == ImagePart::Nine ? DivideType::XY : DivideType::None;
}

void ImageSelector::CheckSourcePartition() noexcept
{
    if (!m_sourceTexture) return;

    const XMINT2& pos = InputManager::GetMouse().GetPosition();
    m_hoveredAreas = m_sourceBinder->GetAreas(m_sourceTexture->GetFilename(), m_selectImagePart, pos);
    if (!m_hoveredAreas.empty()) return; //먼저 저장돼 있는 것을 찾아보고 없으면 새로운걸 만든다.

    if(auto currRectangle = FindAreaFromMousePos(pos); currRectangle)
        m_hoveredAreas = GenerateSourceAreas(*currRectangle, ImagePartToDivideType(m_selectImagePart));
}

void ImageSelector::Render()
{
	SelectImagePart();
    EditSelectArea();
    RenderHoveredArea();
    RenderSelectedArea();
    RenderLabeledAreas();
}

void ImageSelector::SelectImagePart()
{
    if (!m_sourceTexture) return;

    static int selected{ 0 };
    int pre{ selected };
    if (ImGui::RadioButton("1 Part", &selected, 0)) m_selectImagePart = ImagePart::One; ImGui::SameLine();
    if (ImGui::RadioButton("3H Part", &selected, 1)) m_selectImagePart = ImagePart::ThreeH; ImGui::SameLine();
    if (ImGui::RadioButton("3V Part", &selected, 2)) m_selectImagePart = ImagePart::ThreeV; ImGui::SameLine();
    if (ImGui::RadioButton("9 Part", &selected, 3)) m_selectImagePart = ImagePart::Nine;
    if (pre != selected) DeselectArea();
}

void ImageSelector::RenderHoveredArea() const
{
    DrawRectangles(m_textureWindow->GetWindow(), m_hoveredAreas, ToColor(Colors::White), ToColor(Colors::White, 0.3f));
}

void ImageSelector::RenderSelectedArea() const
{
    if (!m_selectedArea) return;
    DrawRectangles(m_textureWindow->GetWindow(), m_selectedArea->sources,
        ToColor(Colors::White), ToColor(Colors::White, 0.3f));
}

void ImageSelector::RenderLabeledAreas() const
{
    if (!m_sourceTexture) return;
    vector<TextureSourceInfo> sourceInfos = m_sourceBinder->GetAreas(m_sourceTexture->GetFilename(), m_selectImagePart);
    for (auto& info : sourceInfos)
        DrawRectangles(m_textureWindow->GetWindow(), info.sources, ToColor(Colors::Blue));
}

static SourceDivider GetSourceDivider(ImagePart imgPart, TextureSourceInfo* sourceInfo) noexcept
{
    const vector<Rectangle>& sources = sourceInfo->sources;
    Rectangle mergedSource = CombineRectangles(sources);

    SourceDivider srcDivider{ mergedSource };
    if (imgPart == ImagePart::One) return srcDivider;

    const Rectangle& leftSource = sources[0];
    const int leftX = leftSource.width;
    const int rightX = sources[2].x - mergedSource.x;
    const int top = leftSource.height;
    const int bottom = sources[(sources.size() == 3) ? 2 : 6].y - mergedSource.y;

    switch (imgPart)
    {
    case ImagePart::ThreeH: srcDivider.list = { leftX, rightX }; break;
    case ImagePart::ThreeV: srcDivider.list = { top, bottom }; break;
    case ImagePart::Nine: srcDivider.list = { leftX, rightX, top, bottom }; break;
    }
    return srcDivider;
}

static vector<Rectangle> GetSources(ImagePart imgPart, const SourceDivider& sourceDivider) noexcept
{
    if (imgPart == ImagePart::One) return { sourceDivider.rect };

    vector<int> widths{}, heights{};
    bool success = false;
    switch (imgPart)
    {
    case ImagePart::ThreeH: success = GetSizeDividedByThree(DirectionType::Horizontal, sourceDivider, widths, heights); break;
    case ImagePart::ThreeV: success = GetSizeDividedByThree(DirectionType::Vertical, sourceDivider, widths, heights); break;
    case ImagePart::Nine: success = GetSizeDividedByNine(sourceDivider, widths, heights); break;
    default: return {};
    }

    return success ? GetSourcesFromArea(sourceDivider.rect, widths, heights) : vector<Rectangle>{};
}

void ImageSelector::EditSelectArea()
{
    if (!m_selectedArea) return;

    const string& bindingKey = m_sourceBinder->GetBindingKey(*m_selectedArea);
    SourceDivider sourceDivider = GetSourceDivider(m_selectImagePart, m_selectedArea.get());
    if (EditSourceAndDivider("Source Area", "Deviders", sourceDivider))
    {
        m_selectedArea->sources = GetSources(m_selectImagePart, sourceDivider);
        m_sourceBinder->SetSourceInfo(bindingKey, *m_selectedArea);
    }
        
    m_renameNotifier->EditName("Tex Bind Key", bindingKey, [this, &bindingKey](const string& newKey) {
        if (bindingKey.empty()) return m_sourceBinder->InsertTextureKey(newKey, *m_selectedArea);
        if (newKey.empty()) return RemoveArea();
        return m_sourceBinder->ModifyTextureKey(bindingKey, newKey);
        });
}