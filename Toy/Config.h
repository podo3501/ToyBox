#pragma once

void InitializeConfig(const wstring& resourcePath, const Vector2& resolution) noexcept;

wstring GetResourceFullFilename(const wstring& filename) noexcept;
const Vector2& GetResolution() noexcept;
Rectangle GetRectResolution() noexcept;
wstring GetRelativePath(const wstring& fullPath) noexcept;