#pragma once

#include "build_command.h"

class boundary_build_command : public build_command
{

public:

  enum class type { normal, portal };

  boundary_build_command(int cx, int cy) : build_command { cx, cy }
  {
  }

  boundary_build_command(int cx, int cy, type commandType) : build_command { cx, cy }, m_type { commandType }
  {
  }

  [[nodiscard]] auto Type() const -> type
  {
    return m_type;
  }

private:

  type m_type { type::normal };

};
