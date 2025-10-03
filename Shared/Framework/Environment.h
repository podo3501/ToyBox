#pragma once
//App을 띄울때 필요한 최소한의 정보를 저장한다. 리소스나 해상도 같은.
//점점 쌓여간다. static 클래스로 만들지 locator를 쓸지 생각해보자.
void InitializeEnvironment(const wstring& resourcePath, const Vector2& resolution) noexcept;

wstring GetResourcePath() noexcept;
wstring GetResourceFontPath() noexcept;
wstring GetResourceFullFilename(const wstring& filename) noexcept;
const Vector2& GetResolution() noexcept;
Rectangle GetRectResolution() noexcept;
wstring GetRelativePath(const wstring& fullPath) noexcept;