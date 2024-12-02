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

template<typename T>
bool WriteJsonFile(unique_ptr<T>& data, const wstring& filename)
{
	JsonOperation writeJ;
	writeJ.Process("Component", data);
	return writeJ.Write(filename);
}

template<typename T>
bool ReadJsonFile(const wstring& filename, unique_ptr<T>& data)
{
	JsonOperation readJ;
	if (!readJ.Read(filename))
		return false;
	readJ.Process("Component", data);
	return true;
}

