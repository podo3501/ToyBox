#pragma once

enum class Origin
{
	Init,
	Center,
	LeftTop,
};

struct ImageSource
{
	wstring filename{};
	vector<Rectangle> list;
};
