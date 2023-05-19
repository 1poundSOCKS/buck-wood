#pragma once

#include "object_input_data.h"
#include "geometry.h"

class level_background
{

public:

  using star_collection = std::vector<game_point>;

  level_background();

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  auto SetCentre(float x, float y) -> void;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_starBrush;
  std::vector<game_point> m_stars;
  float m_x = 0;
  float m_y = 0;
};
