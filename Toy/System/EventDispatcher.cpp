#include "pch.h"
#include "EventDispatcher.h"

void EventDispatcher::Subscribe(const string& region, const string& name, Callback cb) noexcept
{
	auto key = MakeKey(region, name);
	m_subscribers[key].push_back(move(cb));
}

void EventDispatcher::Dispatch(const string& region, const string& name, UIEvent event) noexcept
{
	auto key = MakeKey(region, name);
	auto it = m_subscribers.find(key);
	if (it == m_subscribers.end())
		return;

	for (auto& cb : it->second)
		cb(event);
}

void EventDispatcher::Clear() noexcept
{
	m_subscribers.clear();
}

string EventDispatcher::MakeKey(const string& region, const string& name) 
{ 
	return region + ":" + name; 
}