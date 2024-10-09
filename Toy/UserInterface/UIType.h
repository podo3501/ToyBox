#pragma once

enum class Origin
{
	Center,
	LeftTop,
};

struct ImageSource
{
	wstring filename{};
	vector<Rectangle> list;
};
