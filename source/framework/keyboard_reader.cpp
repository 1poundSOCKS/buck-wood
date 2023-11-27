#include "pch.h"
#include "keyboard_reader.h"

auto keyboard_reader2::create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void
{
  destroy();
  m_instance = new keyboard_reader2 { keyboardDevice };
}

auto keyboard_reader2::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto keyboard_reader2::update() -> void
{
  m_instance->Update();
}

[[nodiscard]] auto keyboard_reader2::pressed(int key) -> bool
{
  return m_instance->Pressed(key);
}

keyboard_reader2::keyboard_reader2(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) : 
  m_keyboardDevice { keyboardDevice }, m_currentState { std::make_unique<keyboard_state>() }, m_previousState { std::make_unique<keyboard_state>() }
{
}

auto keyboard_reader2::Update() -> void
{
  m_previousState.swap(m_currentState);
  m_currentState->Update(m_keyboardDevice.get());
}

[[nodiscard]] auto keyboard_reader2::Pressed(int key) const -> bool
{
  return m_currentState->Down(key) && !m_previousState->Down(key);
}
