#pragma once

class JsonOperation;

class AutoNamer //�̸��� base, base_1... ��, base_0 �̶�� �ڵ� �̸��� ����.
{
public:
    ~AutoNamer();
    AutoNamer();
    //AutoNamer(const AutoNamer& other);
    bool operator==(const AutoNamer& other) const noexcept;

    string Generate() noexcept;
    [[nodiscard]] pair<bool, bool> Recycle(int id) noexcept;
    inline bool IsDeletable() noexcept { return (m_nextID <= m_recycled.size()); }
    bool IsUsed(int id) const noexcept;
    void SerializeIO(JsonOperation& operation);

private:
    inline bool HasBeenGenerated(int id) const noexcept { return id >= 0 && id < m_nextID; }

    int m_nextID{ 0 };
    set<int> m_recycled;
    bool m_isDeletable{ false };
};