#pragma once

void InitializeConfig(const wstring& resourcePath, const Vector2& resolution) noexcept;

const wstring& GetResourcePath() noexcept;
const Vector2& GetResolution() noexcept;