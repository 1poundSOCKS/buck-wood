#pragma once

#include "framework.h"

class game_clock
{

public:

  static auto setMultiplier(float value);
  [[nodiscard]] static auto getInterval(__int64 clockTicks) -> float;

private:

  inline static float m_multiplier { 0 };

};

inline auto game_clock::setMultiplier(float value)
{
  m_multiplier = value;
}

[[nodiscard]] inline auto game_clock::getInterval(__int64 clockTicks) -> float
{
  return static_cast<float>(clockTicks) / static_cast<float>(clock_frequency::get()) * m_multiplier;
}
