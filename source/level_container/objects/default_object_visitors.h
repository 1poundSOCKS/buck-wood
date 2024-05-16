#pragma once

#include "enemy_type_1.h"
#include "enemy_type_2.h"

struct scale_visitor
{
  auto operator()(const auto& object)
  {
    return object.Scale();
  }
};

struct angle_visitor
{
  auto operator()(const auto& object)
  {
    return object.Angle();
  }
};

struct position_visitor
{
  auto operator()(const auto& object)
  {
    return object.Position();
  }
};

struct destroyed_visitor
{
  auto operator()(const auto& object)
  {
    return object.Destroyed();
  }
};

struct destroy_visitor
{
  auto operator()(auto& object)
  {
    return object.Destroy();
  }
};

struct health_visitor
{
  auto operator()(const enemy_type_1& object) -> float
  {
    return object.Health();
  }
  auto operator()(const enemy_type_2& object) -> float
  {
    return object.Health();
  }
  auto operator()(const auto& object) -> float
  {
    return 1.0;
  }
};
