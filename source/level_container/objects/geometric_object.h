#pragma once

#include "framework.h"

class geometric_object
{

public:

  geometric_object(POINT_2F position, SCALE_2F scale, float angle) noexcept : m_position { position }, m_scale { scale }, m_angle { angle }
  {
  }

  [[nodiscard]] auto Position() const noexcept -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Scale() const noexcept -> SCALE_2F
  {
    return m_scale;
  };

  [[nodiscard]] auto Angle() const noexcept -> float
  {
    return m_angle;
  }

  auto SetPosition(POINT_2F value) -> void
  {
    m_position = value;
  }

  auto SetScale(SCALE_2F value) -> void
  {
    m_scale = value;
  }

  auto Move(float distance, float direction) -> void
  {
    auto relativePosition = direct2d::CalculatePosition(distance, direction);
    m_position = direct2d::ShiftPosition(m_position, relativePosition);
  }

  auto MoveTowards(float distance, POINT_2F destination) -> bool
  {
    m_position = direct2d::GetDistanceBetweenPoints(m_position, destination) > distance ? direct2d::CalculatePosition(m_position, direct2d::GetAngleBetweenPoints(m_position, destination), distance) : destination;
    return direct2d::AreEqual(m_position, destination);
  }

  auto RotateBy(float angle) -> void
  {
    m_angle = direct2d::RotateAngle(m_angle, angle);
  }

  [[nodiscard]] auto DirectionTo(POINT_2F target) const -> float
  {
    return direct2d::GetAngleBetweenPoints(m_position, target);
  }

protected:

  POINT_2F m_position;
  SCALE_2F m_scale;
  float m_angle;

};
