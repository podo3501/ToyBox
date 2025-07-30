#pragma once

string AppendIfPresent(const string& base, const string& suffix, const string& delim = "_");
pair<string, int> ExtractNameAndId(string_view name);
