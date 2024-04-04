#pragma once

#include "game_state.h"

class save_data
{

public:

  static auto create(std::wstring folder) noexcept -> void;
  static auto destroy() noexcept -> void;

  static auto read(game_state* gameState) noexcept -> void;
  static auto write(const game_state* gameState) noexcept -> void;

private:

  save_data(std::wstring folder) noexcept;
  auto Read(game_state* gameState) noexcept -> void;
  auto Write(const game_state* gameState) noexcept -> void;

private:

  struct game_state_data
  {
    uint32_t levelIndex { 0 };
    uint32_t score { 0 };
    uint32_t powerups { 0 };
  };

  inline static save_data* m_instance { nullptr };
  std::wstring m_folder;

};

inline auto save_data::create(std::wstring folder) noexcept -> void
{
  destroy();
  m_instance = new save_data(folder);
}

inline auto save_data::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto save_data::read(game_state *gameState) noexcept -> void
{
  m_instance->Read(gameState);
}

inline auto save_data::write(const game_state *gameState) noexcept -> void
{
  m_instance->Write(gameState);
}
