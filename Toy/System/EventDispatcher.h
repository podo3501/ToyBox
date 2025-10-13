#pragma once

enum class UIEvent : int
{
	Clicked,
	Unknown,
};

class EventDispatcher
{
public:
	using Callback = function<void(UIEvent)>;

	static void Subscribe(const string& region, const string& name, Callback cb) noexcept;
	static void Dispatch(const string& region, const string& name, UIEvent event) noexcept;
	static void Clear() noexcept;

private:
	static string MakeKey(const string& region, const string& name);

	inline static unordered_map<string, vector<Callback>> m_subscribers{};
};