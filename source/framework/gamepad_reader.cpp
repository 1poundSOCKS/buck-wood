#include "pch.h"
#include "gamepad_reader.h"
#include "log.h"

auto gamepad_reader::create() -> void
{
  destroy();
  log::write(log::type::info, "creating gamepad reader");
  m_instance = new gamepad_reader {};
}

auto gamepad_reader::destroy() -> void
{
  if( m_instance )
  {
    log::write(log::type::info, "destroying gamepad reader");
    delete m_instance;
    m_instance = nullptr;
  }
}

[[nodiscard]] auto gamepad_reader::connected() -> bool
{
  return m_instance->m_currentState->Connected();
}

auto gamepad_reader::update() -> void
{
  m_instance->Update();
}

[[nodiscard]] auto gamepad_reader::pressed(int button) -> bool
{
  return m_instance->Pressed(button);
}

[[nodiscard]] auto gamepad_reader::up_pressed() -> bool
{
  return m_instance->UpPressed();
}

[[nodiscard]] auto gamepad_reader::down_pressed() -> bool
{
  return m_instance->DownPressed();
}

[[nodiscard]] auto gamepad_reader::left_pressed() -> bool
{
  return m_instance->LetfPressed();
}

[[nodiscard]] auto gamepad_reader::right_pressed() -> bool
{
  return m_instance->RightPressed();
}

[[nodiscard]] auto gamepad_reader::thumb_lx() -> float
{
  return ToFloat(m_instance->m_currentState->ThumbLX(), m_instance->m_stickDeadzone);
}

[[nodiscard]] auto gamepad_reader::thumb_ly() -> float
{
  return ToFloat(m_instance->m_currentState->ThumbLY(), m_instance->m_stickDeadzone);
}

[[nodiscard]] auto gamepad_reader::thumb_rx() -> float
{
  return ToFloat(m_instance->m_currentState->ThumbRX(), m_instance->m_stickDeadzone);
}

[[nodiscard]] auto gamepad_reader::thumb_ry() -> float
{
  return ToFloat(m_instance->m_currentState->ThumbRY(), m_instance->m_stickDeadzone);
}

[[nodiscard]] auto gamepad_reader::left_trigger() -> float
{
  return ToFloat(m_instance->m_currentState->LeftTrigger());
}

[[nodiscard]] auto gamepad_reader::right_trigger() -> float
{
  return ToFloat(m_instance->m_currentState->RightTrigger());
}

[[nodiscard]] auto gamepad_reader::buttons() -> WORD
{
  return m_instance->m_currentState->Buttons();
}

auto gamepad_reader::set_stick_deadzone(float value) -> void
{
  m_instance->m_stickDeadzone = value;
}

gamepad_reader::gamepad_reader() : m_currentState { std::make_unique<gamepad_state>() }, m_previousState { std::make_unique<gamepad_state>() }
{
}

auto gamepad_reader::Update() -> void
{
  m_previousState.swap(m_currentState);
  m_currentState->Update();
}

[[nodiscard]] auto gamepad_reader::Pressed(int button) const -> bool
{
  return m_currentState->Down(button) && !m_previousState->Down(button);
}

[[nodiscard]] auto gamepad_reader::UpPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_UP);
}

[[nodiscard]] auto gamepad_reader::DownPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_DOWN);
}

[[nodiscard]] auto gamepad_reader::LetfPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_LEFT);
}

[[nodiscard]] auto gamepad_reader::RightPressed() -> bool
{
  return m_instance->Pressed(XINPUT_GAMEPAD_DPAD_RIGHT);
}

[[nodiscard]] auto gamepad_reader::ToFloat(SHORT value, float deadzone) -> float
{
  auto floatValue = ToFloat(value);
  return ( floatValue < deadzone && floatValue > -deadzone ) ? 0 : ScaleToLivezone(floatValue, deadzone);
}

[[nodiscard]] auto gamepad_reader::ToFloat(SHORT value) -> float
{
  constexpr auto positiveShift = -SHRT_MIN;
  auto positiveShiftedValue = static_cast<float>(value) + positiveShift;
  return positiveShiftedValue / positiveShift - 1.0f;
}

[[nodiscard]] auto gamepad_reader::ToFloat(BYTE value) -> float
{
  return static_cast<float>(value) / UCHAR_MAX;
}

[[nodiscard]] auto gamepad_reader::ScaleToLivezone(float floatValue, float deadzone) -> float
{
  auto range = 1.0f - deadzone;
  auto zeroShiftedValue = ( floatValue > 0 ) ? ( floatValue - deadzone ) : ( floatValue + deadzone );
  return zeroShiftedValue / range;
}
