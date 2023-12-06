#pragma once

#include "base_scene.h"

class scene_controller
{

public:

  using scene_ptr = std::unique_ptr<base_scene>;
  using scene_collection = std::vector<scene_ptr>;

  template <typename scene_type, class... Args> auto AddScene(Args&&... args) -> void;

  auto Clear() -> void;
  auto Begin() -> void;
  auto End() -> void;

  auto Pause() const -> void;
  auto Resume() const -> void;

  auto UpdateScene(int64_t ticks) -> void;
  auto RenderScene() const -> void;

  [[nodiscard]] auto Current() const -> base_scene&;
  [[nodiscard]] auto Complete() const -> bool;

private:

  scene_collection m_scenes;
  scene_collection::iterator m_currentScene;

};

template <typename scene_type, class... Args> auto scene_controller::AddScene(Args&&... args) -> void
{
  m_scenes.emplace_back(std::make_unique<scene_type>(std::forward<Args>(args)...));
}

inline auto scene_controller::Clear() -> void
{
  m_scenes.clear();
}

inline auto scene_controller::Begin() -> void
{
  m_currentScene = std::begin(m_scenes);

  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Begin();
  }
}

inline auto scene_controller::End() -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().End();
  }
}

inline auto scene_controller::Pause() const -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Pause();
  }
}

inline auto scene_controller::Resume() const -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Resume();
  }
}

inline auto scene_controller::UpdateScene(int64_t ticks) -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    if( !Current().Update(ticks) )
    {
      Current().End();

      if( ++m_currentScene != std::end(m_scenes) )
      {
        Current().Begin();
      }
    }
  }
}

inline auto scene_controller::RenderScene() const -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Render();
  }
}

inline [[nodiscard]] auto scene_controller::Current() const -> base_scene&
{
  return *(m_currentScene->get());
}

inline [[nodiscard]] auto scene_controller::Complete() const -> bool
{
  return m_currentScene == std::end(m_scenes);
}
