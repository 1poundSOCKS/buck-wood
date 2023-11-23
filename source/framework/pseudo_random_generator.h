#pragma once

class pseudo_random_generator
{

public:

  static auto seed(unsigned int value) -> void;
  [[nodiscard]] static auto get() -> std::mt19937&;

private:

  inline static std::mt19937 m_rng;

};

inline auto pseudo_random_generator::seed(unsigned int value) -> void
{
  m_rng.seed(value);
}

[[nodiscard]] inline auto pseudo_random_generator::get() -> std::mt19937&
{
  return m_rng;
}
