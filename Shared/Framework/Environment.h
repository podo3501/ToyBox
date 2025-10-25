#pragma once
//App�� ��ﶧ �ʿ��� �ּ����� ������ �����Ѵ�. ���ҽ��� �ػ� ����.
class Environment
{
public:
	Environment() = delete;
	Environment(const wstring& resourcePath, const Vector2& resolution);

	const wstring& GetResourcePath() const noexcept { return m_resourcePath; }
	const wstring& GetResourceFontPath() const noexcept { return m_fontPath; }
	const Vector2& GetResolution() const noexcept { return m_resolution; }
	Rectangle GetRectResolution() const noexcept;
	wstring GetResourceFullFilename(const wstring& filename) const noexcept;
	wstring GetRelativePath(const wstring& fullPath) const noexcept;

private:
	wstring m_fontPath;
	wstring m_resourcePath;
	Vector2 m_resolution{};
};