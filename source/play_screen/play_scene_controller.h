#pragma once

#include "play_scene.h"

class play_scene_controller
{

public:

  using play_scene_ptr = std::unique_ptr<play_scene>;
  using play_scene_collection = std::vector<play_scene_ptr>;

  auto AddScene(play_scene_ptr playScene) -> void;
  template <typename scene_type, class... Args> auto AddScene(Args&&... args) -> void;
  auto Clear() -> void;
  auto Begin() -> void;
  auto RefreshScene(int64_t ticks) -> void;

  [[nodiscard]] auto Current() const -> play_scene&;
  [[nodiscard]] auto Complete() const -> bool;

private:

  play_scene_collection m_scenes;
  play_scene_collection::iterator m_currentScene;

};

inline auto play_scene_controller::AddScene(play_scene_ptr playScene) -> void
{
  m_scenes.push_back(std::move(playScene));
}

template <typename scene_type, class... Args> auto play_scene_controller::AddScene(Args&&... args) -> void
{
  std::unique_ptr<scene_type> playScene = std::make_unique<scene_type>(std::forward<Args>(args)...);
  m_scenes.emplace_back(std::move(playScene));
}

inline auto play_scene_controller::Clear() -> void
{
  m_scenes.clear();
}

inline auto play_scene_controller::Begin() -> void
{
  m_currentScene = std::begin(m_scenes);
}

inline [[nodiscard]] auto play_scene_controller::Current() const -> play_scene&
{
  return *(m_currentScene->get());
}

inline auto play_scene_controller::RefreshScene(int64_t ticks) -> void
{
  if( m_currentScene != std::end(m_scenes) )
  {
    auto& currentScene = *m_currentScene;

    if( !currentScene->Refresh(ticks) )
    {
      currentScene->End();

      if( ++m_currentScene != std::end(m_scenes) )
      {
        (*m_currentScene)->Begin();
      }
    }
  }
}

inline [[nodiscard]] auto play_scene_controller::Complete() const -> bool
{
  return m_currentScene == std::end(m_scenes);
}
