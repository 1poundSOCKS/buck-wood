#pragma once

#include "game_state.h"

class save_data
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static auto write(const game_state* gameState) -> void;

private:

  auto Write(const game_state* gameState) -> void;

private:

  inline static save_data* m_instance { nullptr };

};

inline auto save_data::create() -> void
{
  destroy();
  m_instance = new save_data();
}

inline auto save_data::destroy() -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto save_data::write(const game_state* gameState) -> void
{
  m_instance->Write(gameState);
}
