#pragma once

class game_state
{

public:

  static auto create() noexcept -> void;
  static auto destroy() noexcept -> void;

  [[nodiscard]] static auto saved() noexcept -> bool;

private:

  [[nodiscard]] auto Saved() noexcept -> bool;

private:

  inline static game_state* m_instance { nullptr };

};

inline auto game_state::create() noexcept -> void
{
  destroy();
  m_instance = new game_state();
}

inline auto game_state::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto game_state::saved() noexcept -> bool
{
  return m_instance->Saved();
}
