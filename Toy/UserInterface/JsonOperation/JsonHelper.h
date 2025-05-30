#pragma once
#include "JsonOperation.h"

class UIComponent;
//?!?  이 클래스가 UIModule이 만들어지면 사라지게 될 것이다. Module 클래스가 이 클래스를 대신해야한다.
class JsonFile
{
public:
	template<typename T>
	static bool Read(const wstring& filename, T& data)
	{
		JsonOperation readJ;
		if (!readJ.Read(filename)) 
			return false;
		data.SerializeIO(readJ);

		return true;
	}

	template<typename T>
	static bool Write(T& data, const wstring& filename)
	{
		JsonOperation writeJ;
		data.SerializeIO(writeJ);
		return writeJ.Write(filename);
	}

	template<typename T>
	static bool WriteComponent(T data, const wstring& filename)
	{
		JsonOperation writeJ;
		writeJ.Process("Component", data);
		wstring curFilename = GetJsonFilename(data);
		if (!filename.empty())
			curFilename = filename;
		auto result = writeJ.Write(curFilename);
		if (result)
			SetJsonFilename(data, curFilename);

		return result;
	}

	//?!? 2개가 있다 하나 지울것 Read 문제가 해결되면.
	template<typename T>
	static bool WriteComponent(unique_ptr<T>& data, const wstring& filename)
	{
		JsonOperation writeJ;
		writeJ.Process("Component", data);
		wstring curFilename = GetJsonFilename(data.get());
		if (!filename.empty())
			curFilename = filename;
		auto result = writeJ.Write(curFilename);
		if (result)
			SetJsonFilename(data.get(), curFilename);

		return result;
	}

	template<typename T>
	static bool ReadComponent(const wstring& filename, unique_ptr<T>& data)
	{
		JsonOperation readJ;
		if (!readJ.Read(filename))
			return false;
		readJ.Process("Component", data); //?!? 이게 template<IsNotUIComponent T> 이쪽 Process로 가는데 문제가 있다.
		SetJsonFilename(data.get(), filename);
		return true;
	}

	static wstring GetJsonFilename(const UIComponent* data) noexcept;

private:
	static void SetJsonFilename(const UIComponent* data, const wstring& filename) noexcept;

	static map<const UIComponent*, wstring> m_filenames;
};


