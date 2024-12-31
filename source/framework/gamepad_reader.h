#pragma once

#include "gamepad_state.h"

class gamepad_reader
{

public:

  using thumbstick_position = std::optional<D2D1_POINT_2F>;

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto connected() -> bool;

  static auto update() -> void;
  
  static [[nodiscard]] auto button_down(int button) -> bool;
  static [[nodiscard]] auto button_pressed(int button) -> bool;
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

  static auto set_stick_deadzone(float value) -> void;
  static auto invert_y_axis() -> void;

  static [[nodiscard]] auto left_thumbstick() -> thumbstick_position;
  static [[nodiscard]] auto right_thumbstick() -> thumbstick_position;

private:

  gamepad_reader();
  auto Update() -> void;

  [[nodiscard]] auto ButtonPressed(int button) const -> bool;
  [[nodiscard]] auto UpPressed() -> bool;
  [[nodiscard]] auto DownPressed() -> bool;
  [[nodiscard]] auto LetfPressed() -> bool;
  [[nodiscard]] auto RightPressed() -> bool;

  static [[nodiscard]] auto ToFloat(SHORT value, float deadzone) -> float;
  static [[nodiscard]] auto ToFloat(SHORT value) -> float;
  static [[nodiscard]] auto ToFloat(BYTE value) -> float;
  static [[nodiscard]] auto ScaleToLivezone(float floatValue, float deadzone) -> float;

  static [[nodiscard]] auto GetStickCoordinates(float angle, float distance) -> D2D1_POINT_2F;

private:

  inline static gamepad_reader* m_instance { nullptr };
  std::unique_ptr<gamepad_state> m_currentState;
  std::unique_ptr<gamepad_state> m_previousState;
  float m_stickDeadzone { 0.2f };
  bool m_yAxisInverted { false };
};
