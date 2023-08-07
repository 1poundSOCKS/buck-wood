#pragma once

#include <xinput.h>

class gamepad_state
{

public:

  gamepad_state() = default;
  gamepad_state(DWORD userIndex);

  auto Update() -> DWORD;

  [[nodiscard]] auto Connected() const -> bool;

  [[nodiscard]] auto Buttons() const -> WORD;
  [[nodiscard]] auto LeftTrigger() const -> BYTE;
  [[nodiscard]] auto RightTrigger() const -> BYTE;
  [[nodiscard]] auto ThumbLX() const -> SHORT;
  [[nodiscard]] auto ThumbLY() const -> SHORT;
  [[nodiscard]] auto ThumbRX() const -> SHORT;
  [[nodiscard]] auto ThumbRY() const -> SHORT;

  [[nodiscard]] auto ThumbLXRatio() const -> float;
  [[nodiscard]] auto ThumbLYRatio() const -> float;
  [[nodiscard]] auto ThumbRXRatio() const -> float;
  [[nodiscard]] auto ThumbRYRatio() const -> float;

private:

  [[nodiscard]] static auto Ratio(SHORT value) -> float;

  DWORD m_userIndex { 0 };
  bool m_connected { false };
  XINPUT_STATE m_state;

};

inline gamepad_state::gamepad_state(DWORD userIndex) : m_userIndex { userIndex }
{
  if( m_userIndex < 0 || m_userIndex > 3 )
  {
    throw std::exception();
  }
}

inline auto gamepad_state::Update() -> DWORD
{
  auto stateReturn = XInputGetState(m_userIndex, &m_state);
  m_connected = stateReturn == ERROR_SUCCESS ? true : false;
  return stateReturn;
}

[[nodiscard]] inline auto gamepad_state::Connected() const -> bool
{
  return m_connected;
}

[[nodiscard]] inline auto gamepad_state::Buttons() const -> WORD
{
  return m_state.Gamepad.wButtons;
}

[[nodiscard]] inline auto gamepad_state::LeftTrigger() const -> BYTE
{
  return m_state.Gamepad.bLeftTrigger;
}

[[nodiscard]] inline auto gamepad_state::RightTrigger() const -> BYTE
{
  return m_state.Gamepad.bRightTrigger;
}

[[nodiscard]] inline auto gamepad_state::ThumbLX() const -> SHORT
{
  return m_state.Gamepad.sThumbLX;
}

[[nodiscard]] inline auto gamepad_state::ThumbLY() const -> SHORT
{
  return m_state.Gamepad.sThumbLY;
}

[[nodiscard]] inline auto gamepad_state::ThumbRX() const -> SHORT
{
  return m_state.Gamepad.sThumbRX;
}

[[nodiscard]] inline auto gamepad_state::ThumbRY() const -> SHORT
{
  return m_state.Gamepad.sThumbRY;
}

[[nodiscard]] inline auto gamepad_state::ThumbLXRatio() const -> float
{
  return Ratio(ThumbLX());
}

[[nodiscard]] inline auto gamepad_state::ThumbLYRatio() const -> float
{
  return Ratio(ThumbLY());
}

[[nodiscard]] inline auto gamepad_state::ThumbRXRatio() const -> float
{
  return Ratio(ThumbRX());
}

[[nodiscard]] inline auto gamepad_state::ThumbRYRatio() const -> float
{
  return Ratio(ThumbRY());
}

[[nodiscard]] inline auto gamepad_state::Ratio(SHORT value) -> float
{
  constexpr auto positiveShift = -SHRT_MIN;
  auto positiveShiftedValue = static_cast<float>(value) + positiveShift;
  return positiveShiftedValue / positiveShift - 1.0f;
}
