#pragma once
//����� ���� ������ �� ������Ʈ���� pch �� ����� ����ִٰ� �����ϱ� �����̴�. ������ ���ٸ� ���⸻�� ������ ������Ʈ���� include ����.

template <typename T>
using optionalRef = std::optional<std::reference_wrapper<const T>>;