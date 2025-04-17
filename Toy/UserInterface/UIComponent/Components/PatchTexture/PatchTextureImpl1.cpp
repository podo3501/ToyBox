#include "pch.h"
#include "PatchTextureImpl1.h"
#include "../Include/IRenderer.h"
#include "Utility.h"

//template class PatchTextureImpl1<PatchTexture1>; //명시적 인스턴스화. 즉 이 템플릿 클래스는 이것만 만든다.

//template<typename TextureType>
//PatchTextureImpl1<TextureType>::PatchTextureImpl1(const PatchTextureImpl1& o) :
//	m_index{ o.m_index },
//	m_source{ o.m_source }
//{}

//template<typename TextureType>
//inline TextureType* Self() { return static_cast<TextureType*>(this); }
//
//template<typename TextureType>
//inline const TextureType* Self() { return static_cast<const TextureType*>(this); }

//template<typename TextureType>
//void PatchTextureImpl1<TextureType>::ImplementRenderImpl(ITextureRender* render) const
//{
//	const auto& position = static_cast<TextureType*>(this)->GetPosition();
//	//const auto& size = static_cast<const TextureType*>(this)->GetSize();
//	//Rectangle destination(position.x, position.y, size.x, size.y);
//
//	//RECT source = RectangleToRect(m_source);
//	//render->Render(*m_index, destination, &source);
//}
//
//template<typename TextureType>
//void PatchTextureImpl1<TextureType>::SetIndexedSourceImpl(size_t index, const vector<Rectangle>& source) noexcept
//{
//	m_index = index;
//	m_source = source[0];
//}