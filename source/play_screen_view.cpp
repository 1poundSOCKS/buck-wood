#include "pch.h"
#include "play_screen_view.h"
#include "level_transform_transition.h"
#include "performance_counter.h"

play_screen_view::play_screen_view()
{
  m_totalTicks = performance_counter::QueryFrequency() * 3;
}

auto play_screen_view::SetRenderTargetSize(D2D1_SIZE_F renderTargetSize) -> void
{
  m_renderTargetSize = renderTargetSize;
}

[[nodiscard]] auto play_screen_view::CanPauseScreen() const -> bool
{
  return m_view != view_ending;
}

auto play_screen_view::Update(int64_t elapsedTicks) -> void
{
  switch( m_view )
  {
  case view_starting:
  case view_player_dead:
  case view_ending:
    m_ticks += elapsedTicks;
    break;
  }
}

[[nodiscard]] auto play_screen_view::GetElapsedTicks(int64_t frameTicks) const -> int64_t
{
  return ( m_view == view_starting || m_view == view_ending ) ? 0 : frameTicks;
}

auto play_screen_view::SetPlayerPosition(float x, float y) -> void
{
  m_playerX = x;
  m_playerY = y;
}

[[nodiscard]] auto play_screen_view::GetTransform() const -> D2D1::Matrix3x2F
{
  static const float playZoom = 1.0f;

  if( m_view == view_starting )
  {
    level_transform_transition levelTransformTransiton(0.0f, 0.0f, 0.01f, m_playerX, m_playerY, playZoom);
    return levelTransformTransiton.Get(m_renderTargetSize.width, m_renderTargetSize.height, m_totalTicks, m_ticks);
  }
  else if( m_view == view_ending )
  {
    level_transform_transition levelTransformTransiton(m_playerX, m_playerY, playZoom, 0.0f, 0.0f, 0.01f);
    return levelTransformTransiton.Get(m_renderTargetSize.width, m_renderTargetSize.height, m_totalTicks, m_ticks);
  }
  else
  {
    return CreateTransform(m_playerX, m_playerY, playZoom, m_renderTargetSize.width, m_renderTargetSize.height);
  }
}

[[nodiscard]] auto play_screen_view::IsStarting() const -> bool
{
  return m_view == view_starting;
}

auto play_screen_view::EndPlay() -> void
{
  if( m_view == view_playing )
  {
    Switch();
  }
}

[[nodiscard]] auto play_screen_view::TimeToSwitch() const -> bool
{
  switch( m_view )
  {
    case view_starting:
    case view_player_dead:
    case view_ending:
      return m_ticks >= m_totalTicks;

    default:
      return false;
  }
}

auto play_screen_view::Switch() -> void
{
  auto view = GetNextView();
  if( view != m_view )
  {
    m_view = view;
    m_ticks = 0;
  }
}

auto play_screen_view::ScreenCanClose() const -> bool
{
  return m_view == view_ending && m_ticks >= m_totalTicks;
}

[[nodiscard]] auto play_screen_view::GetNextView() const -> view_type
{
  switch( m_view )
  {
    case view_starting:
      return view_playing;

    case view_playing:
      return view_player_dead;

    case view_player_dead:
      return view_ending;

    default:
      return view_ending;
  }
}

[[nodiscard]] auto play_screen_view::CreateTransform(float centerPosX, float centerPosY, float scale, 
  float renderTargetWidth, float renderTargetHeight) -> D2D1::Matrix3x2F
{
  return D2D1::Matrix3x2F::Translation(-centerPosX, -centerPosY) * 
    D2D1::Matrix3x2F::Scale(scale, scale) *
    D2D1::Matrix3x2F::Translation(renderTargetWidth / 2, renderTargetHeight / 2);
}

[[nodiscard]] auto play_screen_view::GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data
{
  auto transform = GetTransform();

  if( transform.Invert() )
  {
    auto mousePosition = transform.TransformPoint({ screenInputState.renderTargetMouseData.x, screenInputState.renderTargetMouseData.y });
    auto previousMousePosition = transform.TransformPoint({ screenInputState.previousRenderTargetMouseData.x, screenInputState.previousRenderTargetMouseData.y });

    object_input_data objectInputData;
    objectInputData.SetMouseData({mousePosition.x, mousePosition.y, screenInputState.windowData.mouse.leftButtonDown, screenInputState.windowData.mouse.rightButtonDown});
    objectInputData.SetPreviousMouseData({previousMousePosition.x, previousMousePosition.y, screenInputState.previousWindowData.mouse.leftButtonDown, screenInputState.previousWindowData.mouse.rightButtonDown});
    return objectInputData;    
  }
  else
  {
    return {};
  }
}

[[nodiscard]] auto play_screen_view::GetViewRect() const -> D2D1_RECT_F
{
  auto transform = GetTransform();

  if( transform.Invert() )
  {
    auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
    auto renderTargetBottomRight  = D2D1_POINT_2F { m_renderTargetSize.width - 1.0f, m_renderTargetSize.height - 1.0f };

    auto topLeft = transform.TransformPoint(renderTargetTopLeft);
    auto bottomRight = transform.TransformPoint(renderTargetBottomRight);
    return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
  }
  else
  {
    return { 0, 0, 0, 0 };
  }
}
