#pragma once
#include "../Include/IRenderer.h"

class MockRender : public IRender
{
public:
	virtual ~MockRender() = default;

	MOCK_METHOD(void, Render, (size_t index, const RECT& dest, const RECT* source, bool selected), (override));
	MOCK_METHOD(void, DrawString, 
		(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color), (const override));
};

class MockUpdate : public IGetValue
{
public:
	virtual ~MockUpdate() = default;

	MOCK_METHOD(Rectangle, MeasureText, (size_t index, const wstring& text, const Vector2& position), (override));
};
