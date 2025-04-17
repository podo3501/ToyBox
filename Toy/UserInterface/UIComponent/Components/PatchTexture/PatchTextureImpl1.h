#pragma once

struct ITextureRender;

template<typename TextureType>
class PatchTextureImpl1
{
protected:
	//~PatchTextureImpl1() = default; //CRTP�� ���� ���(Static Polymorphism)�� ��. virtual�� ������ vtable�� ����(�������߻�)
	//PatchTextureImpl1() = default;
	//PatchTextureImpl1(const PatchTextureImpl1& o) = default;

	void ImplementRenderImpl(ITextureRender* render) const
	{
		render;
		//static_cast<const TextureType*>(this)->GetPosition();
		//const auto& position = Self()->GetPosition();
	}

	//void SetIndexedSourceImpl(size_t index, const vector<Rectangle>& source) noexcept;

	optional<size_t> m_index; //�ؽ��� �ε���. 0���� �ε����� ����ϱ� ������ optional
	Rectangle m_source{};

	//inline TextureType* Self() { return static_cast<TextureType*>(this); }
	//inline TextureType* Self() const { return static_cast<const TextureType*>(this); }
};
