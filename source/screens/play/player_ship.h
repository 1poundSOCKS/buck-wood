#ifndef _player_ship_
#define _player_ship_

#include "game_objects.h"
#include "player_control_state.h"
#include "screen_render_data.h"
#include "play_event.h"

struct player_ship
{
  player_ship(screen_render_brush_selector brushes);

  auto Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto HasCollided(float x, float y) const -> bool;
  auto HitByBullet() -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  void RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const;

  void UpdateShipGeometryData();

  [[nodiscard]] auto GetPlayerShipLineData() const -> std::vector<game_line>;

  auto GetTransformedThrusterGeometry(auto pointsInserter) const -> void
  {
    const auto& thrusterGeometryData = GetPlayerThrusterGeometryData();

    TransformPoints(
      thrusterGeometryData.cbegin(), 
      thrusterGeometryData.cend(), 
      pointsInserter, 
      D2D1::Matrix3x2F::Rotation(data->angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(data->xPos, data->yPos)
    );
  }

  auto GetTransformedShipPointsGeometry(auto pointsInserter) const -> void
  {
    const auto& shipGeometryData = GetPlayerGeometryData();

    TransformPoints(
      shipGeometryData.cbegin(), 
      shipGeometryData.cend(), 
      pointsInserter, 
      D2D1::Matrix3x2F::Rotation(data->angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(data->xPos, data->yPos));
  }

  [[nodiscard]] auto PlayerCanShoot(int64_t tickCount) const -> bool;

  enum state_type { alive, dead };

  struct data_type
  {
    state_type state = alive;
    float xPos = 0;
    float yPos = 0;
    float xVelocity = 0;
    float yVelocity = 0;
    float angle = 0;
    bool thrusterOn = false;
    std::vector<game_point> points;
    std::vector<game_line> lines;
    std::shared_ptr<player_control_state> controlState;
    winrt::com_ptr<ID2D1SolidColorBrush> shipBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> thrusterBrush;
  };

  std::shared_ptr<data_type> data;
};

struct event_player_shot
{
  float x = 0;
  float y = 0;
  float angle = 0;
};

#endif
