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
