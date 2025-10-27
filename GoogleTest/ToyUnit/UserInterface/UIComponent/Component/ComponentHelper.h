#pragma once
#include "Shared/SerializerIO/SerializerIO.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

template <typename T>
bool TestWriteAndRead(unique_ptr<T>& component, const wstring& filename)
{
	ReturnIfFalse(SerializerIO::WriteJsonToFile(component, filename));

	unique_ptr<T> read;
	ReturnIfFalse(SerializerIO::ReadJsonFromFile(filename, read));

	return CompareDerived(component, read);
}
