#pragma once

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
  [[nodiscard]] auto IsPlaying() const -> bool;
  [[nodiscard]] auto IsEnding() const -> bool;
  [[nodiscard]] auto TimeToSwitch() const -> bool;
  auto Switch() -> void;
  auto SwitchToEnding() -> void;
  [[nodiscard]] auto ScreenCanClose() const -> bool;

private:

  enum view_type { view_starting, view_playing, view_ending };

  [[nodiscard]] auto GetNextView() const -> view_type;
  
  [[nodiscard]] static auto CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, 
    float renderTargetWidth, float renderTargetHeight) -> D2D1::Matrix3x2F;

  view_type m_view = view_starting;

  D2D1_SIZE_F m_renderTargetSize;

  int64_t m_totalStartingTicks = 0;
  int64_t m_startingTicks = 0;

  int64_t m_totalEndingTicks = 0;
  int64_t m_endingTicks = 0;

  float m_playerX = 0;
  float m_playerY = 0;
};
