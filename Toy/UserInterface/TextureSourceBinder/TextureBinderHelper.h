#pragma once
#include "../Include/TypeAliases.h"

enum class ImagePart;
struct SourceDivider;
struct TextureSourceInfo;
class TextureSourceBinder;

optionalRef<vector<Rectangle>> GetRectangles(TextureSourceBinder* sb, const string& key) noexcept;
Rectangle GetTextureArea(TextureSourceBinder* sb, const string& key, int index) noexcept;
SourceDivider GetSourceDivider(TextureSourceBinder* sb, const string& key) noexcept;
SourceDivider GetSourceDivider(const TextureSourceInfo& sourceInfo) noexcept;;
bool ModifyTextureSourceInfo(TextureSourceBinder* sb, const string& key, const SourceDivider& srcDivider) noexcept;