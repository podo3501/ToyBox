#pragma once

class UILayout;
class ImagePart;
class JsonOperation;
struct ImageSource;
struct ILoadData;
struct IRender;

class ImagePartSet
{
public:
	ImagePartSet();
	~ImagePartSet();
	ImagePartSet(const ImagePartSet& other);
	ImagePartSet(const vector<ImageSource>& imgSources);
	ImagePartSet(const ImageSource& source);
	bool operator==(const ImagePartSet& o) const noexcept;
	string GetType() const;

	//bool SetResources(const wstring& filename);
	bool LoadResources(ILoadData* load);
	bool Update(const Vector2& position) noexcept;
	void Render(IRender* render);
	bool IsPicking(const Vector2& pos)  const noexcept;
	const Rectangle& GetArea() const noexcept;

	bool SetDestination(const Rectangle& area) noexcept;
	bool IsHover(int mouseX, int mouseY) noexcept;
	
	void SerializeIO(JsonOperation* operation);

private:
	void CreateImagePart(const ImageSource& imgSource);
	vector<Rectangle> StretchSize(const Rectangle& area) noexcept;
	inline XMUINT2 GetSize(int vecIdx) const noexcept;

	vector<unique_ptr<ImagePart>> m_images;
};
