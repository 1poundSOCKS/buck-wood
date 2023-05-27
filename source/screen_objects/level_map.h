#pragma once

#include "object_input_data.h"
#include "geometry.h"

class level_map
{

public:

  level_map();

  auto SetRect(game_rect rect) -> void;
  auto SetPlayer(float x, float y) -> void;
  auto SetTargets(std::ranges::input_range auto&& targets) -> void;

  auto Update(const object_input_data& inputData, int64_t clockCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

private:
  
  struct data
  {
    game_rect rect { { 0, 0 }, { 0, 0 } };
    float playerX { 0 };
    float playerY { 0 };
    std::vector<game_point> targets;
  };

  std::shared_ptr<data> m_data;

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;

};

auto level_map::SetTargets(std::ranges::input_range auto&& targets) -> void
{
  auto targetCount = std::ranges::size(targets);
  m_data->targets.reserve(targetCount);
  
  std::ranges::for_each(targets, [this](const auto& target) -> void
  {
    m_data->targets.emplace_back(target);
  });
}
