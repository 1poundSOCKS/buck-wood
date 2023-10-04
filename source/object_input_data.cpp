#include "pch.h"
#include "object_input_data.h"

object_input_data::object_input_data()
{
}

auto object_input_data::SetMouseData(const mouse_data& mouseData) -> void
{
  m_mouseData = mouseData;
}

auto object_input_data::GetMouseData() const -> const mouse_data&
{
  return m_mouseData;
}

auto object_input_data::SetPreviousMouseData(const mouse_data& mouseData) -> void
{
  m_previousMouseData = mouseData;
}

auto object_input_data::GetPreviousMouseData() const -> const mouse_data&
{
  return m_previousMouseData;
}

[[nodiscard]] auto object_input_data::LeftMouseButtonClicked() const -> bool
{
  return m_previousMouseData.leftButtonDown && !m_mouseData.leftButtonDown;
}

[[nodiscard]] auto object_input_data::RightMouseButtonClicked() const -> bool
{
  return m_previousMouseData.rightButtonDown && !m_mouseData.rightButtonDown;
}
