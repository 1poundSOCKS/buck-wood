#pragma once

#include "play_scene.h"

class play_scene_controller
{

public:

  using play_scene_ptr = std::unique_ptr<play_scene>;
  using play_scene_collection = std::vector<play_scene_ptr>;

  template <typename scene_type, class... Args> auto AddScene(Args&&... args) -> void;

  auto Clear() -> void;
  auto Begin() -> void;

  auto Pause() const -> void;
  auto Resume() const -> void;

  auto UpdateScene(int64_t ticks) -> void;
  auto RenderScene() const -> void;

  [[nodiscard]] auto Current() const -> play_scene&;
  [[nodiscard]] auto Complete() const -> bool;

private:

  play_scene_collection m_scenes;
  play_scene_collection::iterator m_currentScene;

};

template <typename scene_type, class... Args> auto play_scene_controller::AddScene(Args&&... args) -> void
{
  m_scenes.emplace_back(std::make_unique<scene_type>(std::forward<Args>(args)...));
}

inline auto play_scene_controller::Clear() -> void
{
  m_scenes.clear();
}

inline auto play_scene_controller::Begin() -> void
{
  m_currentScene = std::begin(m_scenes);
  Current().Begin();
}

inline auto play_scene_controller::Pause() const -> void
{
  Current().Pause();
}

inline auto play_scene_controller::Resume() const -> void
{
  Current().Resume();
}

inline auto play_scene_controller::UpdateScene(int64_t ticks) -> void
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

inline auto play_scene_controller::RenderScene() const -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    Current().Render();
  }
}

inline [[nodiscard]] auto play_scene_controller::Current() const -> play_scene&
{
  return *(m_currentScene->get());
}

inline [[nodiscard]] auto play_scene_controller::Complete() const -> bool
{
  return m_currentScene == std::end(m_scenes);
}
