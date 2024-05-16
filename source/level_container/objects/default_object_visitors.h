#pragma once

struct scale_visitor
{
  auto operator()(const enemy_type_2& object)
  {
    return object.Scale();
  }
  auto operator()(const player_ship& object)
  {
    return object.Scale();
  }
  auto operator()(const enemy_bullet_1& object)
  {
    return object.Scale();
  }
  auto operator()(const auto& object)
  {
    return object.Scale();
  }
};

struct angle_visitor
{
  auto operator()(const enemy_type_2& object)
  {
    return object.Angle();
  }
  auto operator()(const player_ship& object)
  {
    return object.Angle();
  }
  auto operator()(const enemy_bullet_1& object)
  {
    return object.Angle();
  }
  auto operator()(const auto& object)
  {
    return object.Angle();
  }
};

struct position_visitor
{
  auto operator()(const enemy_type_2& object)
  {
    return object.Position();
  }
  auto operator()(const player_ship& object)
  {
    return object.Position();
  }
  auto operator()(const enemy_bullet_1& object)
  {
    return object.Position();
  }
  auto operator()(const auto& object)
  {
    return object.Position();
  }
};

struct destroyed_visitor
{
  auto operator()(const enemy_type_2& object)
  {
    return object.Destroyed();
  }
  auto operator()(const player_ship& object)
  {
    return object.Destroyed();
  }
  auto operator()(const enemy_bullet_1& object)
  {
    return object.Destroyed();
  }
  auto operator()(const auto& object)
  {
    return object.Destroyed();
  }
};

struct destroy_visitor
{
  auto operator()(enemy_type_2& object)
  {
  }
  auto operator()(player_ship& object)
  {
    return object.Destroy();
  }
  auto operator()(enemy_bullet_1& object)
  {
    return object.Destroy();
  }
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
