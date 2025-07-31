#include "pch.h"
#include "AutoNamer.h"
#include "Utility.h"
#include "../JsonOperation/JsonOperation.h"

AutoNamer::~AutoNamer() = default;
AutoNamer::AutoNamer() = default;

//AutoNamer::AutoNamer(const AutoNamer& other)
//    : m_nextID{ other.m_nextID },
//    m_recycled{ other.m_recycled },
//    m_isDeletable{ other.m_isDeletable }
//{}

bool AutoNamer::operator==(const AutoNamer& o) const noexcept
{
    ReturnIfFalse(tie(m_nextID, m_recycled) == tie(o.m_nextID, o.m_recycled));
    return true;
}

string AutoNamer::Generate() noexcept
{
    int id;
    if (!m_recycled.empty())
    {
        auto it = m_recycled.begin();
        id = *it;
        m_recycled.erase(it);
    }
    else
        id = m_nextID++;

    return (id == 0) ? "" : to_string(id);
}

pair<bool, bool> AutoNamer::Recycle(int id) noexcept
{
    if (id >= m_nextID) return { false, false };
    if (!InsertIfAbsent(m_recycled, id)) return { false, false };

    return { true, IsDeletable() };
}

bool AutoNamer::IsUsed(int id) const noexcept
{
    bool generated = HasBeenGenerated(id);
    bool notRecycled = (m_recycled.find(id) == m_recycled.end());

    return generated && notRecycled;
}

void AutoNamer::SerializeIO(JsonOperation& operation)
{
    operation.Process("Namers", m_nextID);
    operation.Process("Recycled", m_recycled);
    operation.Process("Deletable", m_isDeletable);
}
