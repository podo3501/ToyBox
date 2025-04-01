#include "pch.h"
#include "ImageGrid.h"

ImageGrid::~ImageGrid() = default;
ImageGrid::ImageGrid() = default;

ImageGrid::ImageGrid(const ImageGrid& other) :
	UIComponent{ other }
{}