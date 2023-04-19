#ifndef _player_ship_
#define _player_ship_

#include "game_objects.h"
#include "player_control_state.h"
#include "screen_render_data.h"
#include "play_event.h"
#include "bullet.h"
#include "collision_data.h"
#include "collision_effect.h"

class player_ship
{
public:

  enum state_type { alive, dead };

  player_ship();

  auto SetPosition(float x, float y) -> void;
  auto SetThruster(bool thrusterOn) -> void;
  auto SetShoot(bool shoot) -> void;
  auto SetAngle(float angle) -> void;
  auto SetEventShot(std::function<void(float,float,float)> eventShot) -> void;
  auto SetEventDied(std::function<void(float,float)> eventDied) -> void;

  [[nodiscard]] auto GetXPos() const -> float;
  [[nodiscard]] auto GetYPos() const -> float;
  [[nodiscard]] auto GetState() const -> state_type;
  [[nodiscard]] auto ThrusterOn() const -> bool;

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

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

  struct data_type
  {
    state_type state = alive;
    float xPos = 0;
    float yPos = 0;
    float xVelocity = 0;
    float yVelocity = 0;
    float angle = 0;
    bool thrusterOn = false;
    int64_t shotTimerInterval = 0;
    int64_t shotTimer = 0;

    std::vector<game_point> points;
    std::vector<game_line> lines;
    player_control_state controlState;
    winrt::com_ptr<ID2D1SolidColorBrush> shipBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> thrusterBrush;
    std::function<void(float,float,float)> eventShot;
    std::function<void(float,float)> eventDied;
  };

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  screen_render_brush_selector brushes;
  std::shared_ptr<data_type> data;
  collision_data m_collisionData;
};

#endif
