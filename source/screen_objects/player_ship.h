#ifndef _player_ship_
#define _player_ship_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"

class player_ship
{
public:

  enum state_type { alive, dead };

  class control
  {
    friend class player_ship;

  public:

    auto SetEventShot(std::function<void(float,float,float)> eventShot) -> void;
    auto SetEventDied(std::function<void(float,float)> eventDied) -> void;

    [[nodiscard]] auto GetPosition() const -> game_point;
    auto SetPosition(float x, float y) -> void;
    [[nodiscard]] auto ThrusterOn() const -> bool;

  private:

    float m_x = 0;
    float m_y = 0;
    float m_angle = 0;
    bool m_thrusterOn = false;
    std::function<void(float,float,float)> m_eventShot;
    std::function<void(float,float)> m_eventDied;
  };

  using control_data = std::shared_ptr<control>;

  player_ship();
  player_ship(float x, float y);
  auto GetControlData() const -> control_data;

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  using points_collection = std::vector<game_point>;
  using lines_collection = std::vector<game_line>;

  void UpdateShipGeometryData();
  auto GetTransformedThrusterGeometry(std::back_insert_iterator<points_collection> pointsInserter) const -> void;
  auto GetTransformedShipPointsGeometry(std::back_insert_iterator<points_collection> linesInserter) const -> void;

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

  points_collection m_points;
  lines_collection m_lines;
};

#endif
