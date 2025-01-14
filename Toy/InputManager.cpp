#include "pch.h"
#include "InputManager.h"

bool InputManager::m_instantiated = false;

InputManager::~InputManager()
{
    m_instantiated = false;
}

InputManager::InputManager(HWND hwnd)
{
    assert(!m_instantiated);
    m_mouse.SetWindow(hwnd);
    m_instantiated = true;
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