#include "pch.h"
#include "EditUtility.h"

bool EditInteger(const char* label, uint32_t& value, int min, int max)
{
	int temp = value;

	if (!ImGui::InputInt(label, &temp)) return false;
	if (temp < min || temp > max) return false;

	value = temp;
	return true;
}