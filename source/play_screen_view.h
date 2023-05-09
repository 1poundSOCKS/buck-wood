#pragma once

#include "screen_input_state.h"
#include "object_input_data.h"

class play_screen_view
{

public:

  play_screen_view();
  auto SetRenderTargetSize(D2D1_SIZE_F renderTargetSize) -> void;
  [[nodiscard]] auto CanPauseScreen() const -> bool;
  auto Update(int64_t elapsedTicks) -> void;
  [[nodiscard]] auto GetElapsedTicks(int64_t frameTicks) const -> int64_t;
  auto SetPlayerPosition(float x, float y) -> void;
  [[nodiscard]] auto GetTransform() const -> D2D1::Matrix3x2F;
  [[nodiscard]] auto IsStarting() const -> bool;
  auto EndPlay() -> void;
  [[nodiscard]] auto TimeToSwitch() const -> bool;
  auto Switch() -> void;
  [[nodiscard]] auto ScreenCanClose() const -> bool;
  [[nodiscard]] auto GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data;
  [[nodiscard]] auto GetViewRect() const -> D2D1_RECT_F;

private:

  enum view_type { view_starting, view_playing, view_player_dead, view_ending };

  [[nodiscard]] auto GetNextView() const -> view_type;
  
  [[nodiscard]] static auto CreateTransform(float centerPosX, float centerPosY, float scale, 
    float renderTargetWidth, float renderTargetHeight) -> D2D1::Matrix3x2F;

  view_type m_view = view_starting;

  D2D1_SIZE_F m_renderTargetSize;

  int64_t m_totalTicks = 0;
  int64_t m_ticks = 0;

  float m_playerX = 0;
  float m_playerY = 0;
};
