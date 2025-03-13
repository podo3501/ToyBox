#pragma once

struct TextureSourceInfo
{
	wstring filename;
	vector<Rectangle> sources;
};

class TextureSourceBinder
{
public:
	~TextureSourceBinder();
	TextureSourceBinder();
	bool ChangeBindingKey(const string& bindingKey, const vector<Rectangle>& sourceAreas) noexcept;
	string GetBindingKey(const vector<Rectangle>& sourceAreas) const noexcept;

private:
	unordered_map<string, vector<Rectangle>> m_bindingTable;
};

