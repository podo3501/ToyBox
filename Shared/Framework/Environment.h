#pragma once
//App�� ��ﶧ �ʿ��� �ּ����� ������ �����Ѵ�. ���ҽ��� �ػ� ����.
//���� �׿�����. static Ŭ������ ������ locator�� ���� �����غ���.
void InitializeEnvironment(const wstring& resourcePath, const Vector2& resolution) noexcept;

wstring GetResourcePath() noexcept;
wstring GetResourceFontPath() noexcept;
wstring GetResourceFullFilename(const wstring& filename) noexcept;
const Vector2& GetResolution() noexcept;
Rectangle GetRectResolution() noexcept;
wstring GetRelativePath(const wstring& fullPath) noexcept;