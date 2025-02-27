#pragma once

namespace DX { class StepTimer; }

class BoundedValue
{
public:
	BoundedValue() noexcept;
	BoundedValue(int min, int max, int unit) noexcept;
	void SetBounds(int min, int max, int unit) noexcept;
	int GetValue(int wheelValue, const DX::StepTimer& timer) noexcept;
	inline float GetPositionRatio() const noexcept { return static_cast<float>(m_current) / static_cast<float>(m_min); }

private:
	int ValidateRange(int value, double deltaTime) noexcept;

	int m_min{ 0 };
	int m_max{ 0 };
	int m_unit{ 0 };

	int m_target{ 0 };
	double m_current{ 0.f };
	int m_previous{ 0 };
};
