#include "pch.h"
#include "scene_controller.h"

auto scene_controller::Clear() -> void
{
  m_scenes.clear();
}

auto scene_controller::Begin() -> void
{
  m_currentScene = std::begin(m_scenes);

  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Begin();
  }
}

auto scene_controller::End() -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().End();
  }
}

auto scene_controller::Quit() -> void
{
  End();
  m_currentScene = std::end(m_scenes);
}

auto scene_controller::Pause() -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Pause();
  }

  m_paused = true;
}

auto scene_controller::Resume() -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Resume();
  }

  m_paused = false;
}

auto scene_controller::TogglePause() -> bool
{
  m_paused ? Resume() : Pause();
  return m_paused;
}

auto scene_controller::UpdateScene(int64_t ticks) -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    if( !Current().Update(m_paused ? 0 : ticks) )
    {
      Current().End();

      if( ++m_currentScene != std::end(m_scenes) )
      {
        Current().Begin();
      }
    }
  }
}

auto scene_controller::RenderScene() const -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Render();
  }
}

[[nodiscard]] auto scene_controller::Current() const -> base_scene&
{
  return *(m_currentScene->get());
}

[[nodiscard]] auto scene_controller::Complete() const -> bool
{
  return m_currentScene == std::end(m_scenes);
}

[[nodiscard]] auto scene_controller::Paused() const -> bool
{
  return m_paused;
}
