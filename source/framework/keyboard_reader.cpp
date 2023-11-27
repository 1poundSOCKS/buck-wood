#include "pch.h"
#include "keyboard_reader.h"

auto keyboard_reader::create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void
{
  destroy();
  m_instance = new keyboard_reader { keyboardDevice };
}

auto keyboard_reader::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto keyboard_reader::update() -> void
{
  m_instance->Update();
}

[[nodiscard]] auto keyboard_reader::pressed(int key) -> bool
{
  return m_instance->Pressed(key);
}

keyboard_reader::keyboard_reader(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) : 
  m_keyboardDevice { keyboardDevice }, m_currentState { std::make_unique<keyboard_state>() }, m_previousState { std::make_unique<keyboard_state>() }
{
}

auto keyboard_reader::Update() -> void
{
  m_previousState.swap(m_currentState);
  m_currentState->Update(m_keyboardDevice.get());
}

[[nodiscard]] auto keyboard_reader::Pressed(int key) const -> bool
{
  return m_currentState->Down(key) && !m_previousState->Down(key);
}
