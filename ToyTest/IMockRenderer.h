#pragma once
#include "../Include/IRenderer.h"

class MockRender : public IRender
{
public:
	virtual ~MockRender() = default;

	MOCK_METHOD(void, Render, (size_t index, const XMUINT2& size, const Vector2& position, const XMFLOAT2& origin), (override));
};
