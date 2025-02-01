#pragma once
#include<variant>

class ImageGrid1;
class ImageGrid3;
class ImageGrid9;

using ImageGridVariant = std::variant<ImageGrid1*, ImageGrid3*, ImageGrid9*>;
using ImageGrid39Variant = std::variant<ImageGrid3*, ImageGrid9*>;