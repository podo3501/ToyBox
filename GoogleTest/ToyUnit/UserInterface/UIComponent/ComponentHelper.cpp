#include "pch.h"
#include "ComponentHelper.h"
#include "Shared/System/Input/InputManager.h"
#include "Toy/UserInterface/Input/IMouseEventReceiver.h"

DX::StepTimer GetTickTimer() noexcept
{
	DX::StepTimer timer;
	this_thread::sleep_for(std::chrono::milliseconds(3)); //조금 틈을 주지 않으면 틱값이 없어서 시간이 흘러가지 않는다.
	timer.Tick([] {});
	return timer;
}

void SimulateDrag(IMouseEventReceiver* receiver, const XMINT2& start, const XMINT2& end, bool inside) noexcept
{
	receiver->OnPress(start);
	receiver->OnHold(end, inside);
	receiver->OnRelease(inside);
}

