#pragma once

#include "gamepad_state.h"

class gamepad_reader
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto connected() -> bool;

  static auto update() -> void;
  
  static [[nodiscard]] auto pressed(int button) -> bool;  
  static [[nodiscard]] auto up_pressed() -> bool;
  static [[nodiscard]] auto down_pressed() -> bool;
  static [[nodiscard]] auto left_pressed() -> bool;
  static [[nodiscard]] auto right_pressed() -> bool;

  static [[nodiscard]] auto thumb_lx() -> float;
  static [[nodiscard]] auto thumb_ly() -> float;
  static [[nodiscard]] auto thumb_rx() -> float;
  static [[nodiscard]] auto thumb_ry() -> float;
  static [[nodiscard]] auto left_trigger() -> float;
  static [[nodiscard]] auto right_trigger() -> float;

  static [[nodiscard]] auto buttons() -> WORD;

private:

  gamepad_reader();
  auto Update() -> void;

  [[nodiscard]] auto Pressed(int button) const -> bool;
  [[nodiscard]] auto UpPressed() -> bool;
  [[nodiscard]] auto DownPressed() -> bool;
  [[nodiscard]] auto LetfPressed() -> bool;
  [[nodiscard]] auto RightPressed() -> bool;

  static [[nodiscard]] auto ToFloat(SHORT value) -> float;
  static [[nodiscard]] auto ToFloat(BYTE value) -> float;

private:

  inline static gamepad_reader* m_instance { nullptr };
  std::unique_ptr<gamepad_state> m_currentState;
  std::unique_ptr<gamepad_state> m_previousState;

};
