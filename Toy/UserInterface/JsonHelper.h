#pragma once
#include "JsonOperation.h"

template<typename T>
bool ReadJsonFile(const wstring& filename, T& data)
{
	JsonOperation readJ;
	if (!readJ.Read(filename)) 
		return false;
	data.SerializeIO(readJ);

	return true;
}

template<typename T>
bool WriteJsonFile(T& data, const wstring& filename)
{
	JsonOperation writeJ;
	data.SerializeIO(writeJ);
	return writeJ.Write(filename);
}

