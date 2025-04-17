#pragma once

struct ITextureRender;

template<typename TextureType>
class PatchTextureImpl1
{
protected:
	//~PatchTextureImpl1() = default; //CRTP는 정적 상속(Static Polymorphism)을 함. virtual이 있으면 vtable이 생성(오버헤드발생)
	//PatchTextureImpl1() = default;
	//PatchTextureImpl1(const PatchTextureImpl1& o) = default;

	void ImplementRenderImpl(ITextureRender* render) const
	{
		render;
		//static_cast<const TextureType*>(this)->GetPosition();
		//const auto& position = Self()->GetPosition();
	}

	//void SetIndexedSourceImpl(size_t index, const vector<Rectangle>& source) noexcept;

	optional<size_t> m_index; //텍스쳐 인덱스. 0값도 인덱스로 사용하기 때문에 optional
	Rectangle m_source{};

	//inline TextureType* Self() { return static_cast<TextureType*>(this); }
	//inline TextureType* Self() const { return static_cast<const TextureType*>(this); }
};
