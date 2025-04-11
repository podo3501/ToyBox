#include "pch.h"
#include "UIType.h"

TextureSlice DirTypeToTextureSlice(DirectionType dirType) noexcept
{
	switch (dirType)
	{
	case DirectionType::Horizontal: return TextureSlice::ThreeH;
	case DirectionType::Vertical: return TextureSlice::ThreeV;
	default: return TextureSlice::ThreeH;
	}
}