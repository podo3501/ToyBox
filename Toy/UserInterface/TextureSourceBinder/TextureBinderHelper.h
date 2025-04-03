#pragma once
#include "../Include/TypeAliases.h"

enum class ImagePart;
struct SourceDivider;
struct TextureFontInfo;
struct TextureSourceInfo;
class TextureSourceBinder;

optionalRef<vector<Rectangle>> GetRectangles(TextureSourceBinder* sb, const string& key) noexcept;
Rectangle GetTextureArea(TextureSourceBinder* sb, const string& key, int index) noexcept;
SourceDivider GetSourceDivider(TextureSourceBinder* sb, const string& key) noexcept;
SourceDivider GetSourceDivider(const TextureSourceInfo& sourceInfo) noexcept;
pair<wstring, TextureFontInfo> GetTextureFontInfo(TextureSourceBinder* sb, const wstring& filename) noexcept;
vector<pair<string, TextureSourceInfo>> GetTextureSourceInfo(TextureSourceBinder* sb, const wstring& filename) noexcept;
vector<Rectangle> GetSources(ImagePart imgPart, const SourceDivider& sourceDivider) noexcept;