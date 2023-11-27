#pragma once

#include "keyboard_state.h"

class keyboard_reader
{

public:

  static auto create(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice) -> void;
  static auto destroy() -> void;

  static auto update() -> void;
  static [[nodiscard]] auto pressed(int key) -> bool;

private:

  keyboard_reader(const winrt::com_ptr<IDirectInputDevice8>& keyboardDevice);
  auto Update() -> void;
  [[nodiscard]] auto Pressed(int key) const -> bool;

private:

  inline static keyboard_reader* m_instance { nullptr };
  winrt::com_ptr<IDirectInputDevice8> m_keyboardDevice;
  std::unique_ptr<keyboard_state> m_currentState;
  std::unique_ptr<keyboard_state> m_previousState;

};
