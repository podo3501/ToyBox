#include "pch.h"
#include "InputManager.h"

InputManager::~InputManager() = default;
InputManager::InputManager(HWND hwnd)
{
    m_mouse.SetWindow(hwnd);
}

void InputManager::Update()
{
    m_keyboardTracker.Update(m_keyboard.GetState());
    m_mouseTracker.Update(m_mouse.GetState());
}

void InputManager::Update(const Mouse::State& state)
{
    m_mouseTracker.Update(state);
}