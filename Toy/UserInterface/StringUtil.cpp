#include "pch.h"
#include "StringUtil.h"

string AppendIfPresent(const string& base, const string& suffix, const string& delim)
{
    return suffix.empty() ? base : base + delim + suffix;
}

static pair<string_view, string_view> SplitNameAndId(string_view name)
{
    auto underscore = name.find('_');
    if (underscore == string_view::npos) return { name, {} };

    auto prefix = name.substr(0, underscore);
    auto idStr = name.substr(underscore + 1);
    if (idStr.empty() || !std::ranges::all_of(idStr, ::isdigit)) return {};

    return { prefix, idStr };
}

pair<string, int> ExtractNameAndId(string_view name)
{
    auto [prefix, idStr] = SplitNameAndId(name);
    if (prefix.empty()) return { "", 0 };

    int id = (idStr.empty()) ? 0 : stoi(string(idStr));
    return { string(prefix), id };
}