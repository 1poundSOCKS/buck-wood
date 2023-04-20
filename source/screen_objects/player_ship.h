#ifndef _player_ship_
#define _player_ship_

#include "game_objects.h"
#include "player_control_state.h"
#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"

class player_ship
{
public:

  enum state_type { alive, dead };

  class control
  {
    friend class player_ship;

  public:
  
    auto SetPosition(float x, float y) -> void;
    auto SetAngle(float angle) -> void;
    auto SetThruster(bool thrusterOn) -> void;
    auto SetTrigger(bool triggerPressed) -> void;
    auto SetEventShot(std::function<void(float,float,float)> eventShot) -> void;
    auto SetEventDied(std::function<void(float,float)> eventDied) -> void;

    [[nodiscard]] auto GetPosition() const -> game_point;
    [[nodiscard]] auto ThrusterOn() const -> bool;

  private:

    float m_x = 0;
    float m_y = 0;
    float m_angle = 0;
    bool m_thrusterOn = false;
    bool m_triggerPressed = false;
    std::function<void(float,float,float)> m_eventShot;
    std::function<void(float,float)> m_eventDied;
  };

  using control_data = std::shared_ptr<control>;

  player_ship(control_data controlData);

  auto SetPosition(float x, float y) -> void;
  auto SetThruster(bool thrusterOn) -> void;
  auto SetShoot(bool shoot) -> void;
  auto SetAngle(float angle) -> void;

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
      D2D1::Matrix3x2F::Rotation(m_controlData->m_angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(m_controlData->m_x, m_controlData->m_y)
    );
  }

  auto GetTransformedShipPointsGeometry(auto pointsInserter) const -> void
  {
    const auto& shipGeometryData = GetPlayerGeometryData();

    TransformPoints(
      shipGeometryData.cbegin(), 
      shipGeometryData.cend(), 
      pointsInserter, 
      D2D1::Matrix3x2F::Rotation(m_controlData->m_angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(m_controlData->m_x, m_controlData->m_y));
  }

  [[nodiscard]] auto PlayerCanShoot(int64_t tickCount) -> bool;

  control_data m_controlData;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_shipBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_thrusterBrush;

  collision_data m_collisionData;

  state_type m_state = alive;
  float m_velocityX = 0;
  float m_velocityY = 0;
  int64_t m_shotTimerInterval = 0;
  int64_t m_shotTimer = 0;

  std::vector<game_point> m_points;
  std::vector<game_line> m_lines;
};

#endif
