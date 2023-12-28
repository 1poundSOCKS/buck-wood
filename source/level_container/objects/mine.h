#pragma once

#include "framework.h"
#include "homing_object.h"
#include "game_scale.h"

class mine
{

public:

  mine(game_point position);

  [[nodiscard]] auto Scale() const -> game_scale { return game_scale { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval, std::optional<game_point> playerPosition) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto PreviousPosition() const -> const game_point&;

private:

  struct data
  {
    data(game_point position);
    auto Update(float interval, std::optional<game_point> playerPosition) -> void;

    homing_object m_body;
    homing_object m_previousState;
    game_angle m_spin;
    bool m_destroyed { false };
  };

private:

  inline static constexpr float m_spinRate { 500.0f };
  inline static constexpr float m_thrustPower { 50.0f };

  std::shared_ptr<data> m_data;
  
};

inline [[nodiscard]] auto mine::Position() const -> const game_point&
{
  return m_data->m_body.Position();
}

inline [[nodiscard]] auto mine::PreviousPosition() const -> const game_point&
{
  return m_data->m_previousState.Position();
}

inline [[nodiscard]] auto mine::Angle() const -> float
{
  return m_data->m_spin;
}
