#include "pch.h"
#include "gamepad_reader.h"

auto gamepad_reader2::create() -> void
{
  destroy();
  m_instance = new gamepad_reader2 {};
}

auto gamepad_reader2::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

[[nodiscard]] auto gamepad_reader2::connected() -> bool
{
  return m_instance->m_currentState->Connected();
}

auto gamepad_reader2::update() -> void
{
  m_instance->Update();
}

[[nodiscard]] auto gamepad_reader2::pressed(int button) -> bool
{
  return m_instance->Pressed(button);
}

[[nodiscard]] auto gamepad_reader2::up_pressed() -> bool
{
  return m_instance->UpPressed();
}

[[nodiscard]] auto gamepad_reader2::down_pressed() -> bool
{
  return m_instance->DownPressed();
}

[[nodiscard]] auto gamepad_reader2::left_pressed() -> bool
{
  return m_instance->LetfPressed();
}

[[nodiscard]] auto gamepad_reader2::right_pressed() -> bool
{
  return m_instance->RightPressed();
}

[[nodiscard]] auto gamepad_reader2::thumb_lx() -> SHORT
{
  return m_instance->m_currentState->ThumbLX();
}

[[nodiscard]] auto gamepad_reader2::thumb_ly() -> SHORT
{
  return m_instance->m_currentState->ThumbLY();
}

[[nodiscard]] auto gamepad_reader2::thumb_rx() -> SHORT
{
  return m_instance->m_currentState->ThumbRX();
}

[[nodiscard]] auto gamepad_reader2::thumb_ry() -> SHORT
{
  return m_instance->m_currentState->ThumbRY();
}

[[nodiscard]] auto gamepad_reader2::left_trigger() -> BYTE
{
  return m_instance->m_currentState->LeftTrigger();
}

[[nodiscard]] auto gamepad_reader2::right_trigger() -> BYTE
{
  return m_instance->m_currentState->RightTrigger();
}

[[nodiscard]] auto gamepad_reader2::buttons() -> WORD
{
  return m_instance->m_currentState->Buttons();
}

gamepad_reader2::gamepad_reader2() : m_currentState { std::make_unique<gamepad_state>() }, m_previousState { std::make_unique<gamepad_state>() }
{
}

auto gamepad_reader2::Update() -> void
{
  m_previousState.swap(m_currentState);
  m_currentState->Update();
}

[[nodiscard]] auto gamepad_reader2::Pressed(int button) const -> bool
{
  return m_currentState->Down(button) && !m_previousState->Down(button);
}

[[nodiscard]] auto gamepad_reader2::UpPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_UP);
}

[[nodiscard]] auto gamepad_reader2::DownPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_DOWN);
}

[[nodiscard]] auto gamepad_reader2::LetfPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_LEFT);
}

[[nodiscard]] auto gamepad_reader2::RightPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_RIGHT);
}
