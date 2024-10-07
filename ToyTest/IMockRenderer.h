#pragma once
#include "../Include/IRenderer.h"

class MockRender : public IRender
{
public:
	virtual ~MockRender() = default;

	MOCK_METHOD(void, Render, (size_t index, const RECT& dest, const RECT* source), (override));
};
