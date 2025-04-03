#pragma once
#include "../Include/TypeAliases.h"

enum class ImagePart;
struct SourceDivider;
struct TextureFontInfo;
struct TextureSourceInfo;
class TextureResourceBinder;

optionalRef<vector<Rectangle>> GetRectangles(TextureResourceBinder* rb, const string& key) noexcept;
Rectangle GetTextureArea(TextureResourceBinder* rb, const string& key, int index) noexcept;
SourceDivider GetSourceDivider(TextureResourceBinder* rb, const string& key) noexcept;
SourceDivider GetSourceDivider(const TextureSourceInfo& sourceInfo) noexcept;
pair<wstring, TextureFontInfo> GetTextureFontInfo(TextureResourceBinder* rb, const wstring& filename) noexcept;
vector<pair<string, TextureSourceInfo>> GetTextureSourceInfo(TextureResourceBinder* rb, const wstring& filename) noexcept;
vector<Rectangle> GetSources(ImagePart imgPart, const SourceDivider& sourceDivider) noexcept;