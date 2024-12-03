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
	wstring curFilename = data->GetFilename();
	if (!filename.empty())
		curFilename = filename;
	auto result = writeJ.Write(curFilename);
	if (result)
		data->SetFilename(curFilename);

	return result;
}

template<typename T>
bool ReadJsonFile(const wstring& filename, unique_ptr<T>& data)
{
	JsonOperation readJ;
	if (!readJ.Read(filename))
		return false;
	readJ.Process("Component", data);
	data->SetFilename(filename);
	return true;
}


