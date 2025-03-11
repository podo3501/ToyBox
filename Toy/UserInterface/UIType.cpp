#include "pch.h"
#include "UIType.h"
#include "JsonOperation.h"

void ImageSource::SerializeIO(JsonOperation& operation)
{
	operation.Process("Filename", filename);
	operation.Process("SourceList", list);
}
