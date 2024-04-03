#pragma once

class game_state
{

public:

  static auto create() -> void;
  static auto destroy() -> void;
  static auto saved() -> bool;

private:

  inline static game_state* m_instance { nullptr };

};

inline auto game_state::create() -> void
{
  destroy();
  m_instance = new game_state();
}

inline auto game_state::destroy() -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto game_state::saved() -> bool
{
  return false;
}
