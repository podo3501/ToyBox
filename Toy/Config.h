#pragma once
//점점 쌓여간다. static 클래스로 만들지 locator를 쓸지 생각해보자.
namespace StateFlag { enum Type : int; }

void InitializeConfig(const wstring& resourcePath, const Vector2& resolution) noexcept;
void SetRenderFilterFlag(StateFlag::Type filterFlag) noexcept;
StateFlag::Type GetRenderFilterFlag() noexcept;

wstring GetResourceFullFilename(const wstring& filename) noexcept;
const Vector2& GetResolution() noexcept;
Rectangle GetRectResolution() noexcept;
wstring GetRelativePath(const wstring& fullPath) noexcept;