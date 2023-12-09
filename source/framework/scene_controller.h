#pragma once

#include "base_scene.h"

class scene_controller
{

public:

  using scene_ptr = std::unique_ptr<base_scene>;
  using scene_collection = std::vector<scene_ptr>;

  template <typename T, class... Args> auto AddScene(Args&&... args) -> void;

  auto Clear() -> void;
  auto Begin() -> void;
  auto End() -> void;
  auto Quit() -> void;

  auto Pause() -> void;
  auto Resume() -> void;
  auto TogglePause() -> bool;

  auto UpdateScene(int64_t ticks) -> void;
  auto RenderScene() const -> void;

  [[nodiscard]] auto Current() const -> base_scene&;
  [[nodiscard]] auto Complete() const -> bool;
  [[nodiscard]] auto Paused() const -> bool;

private:

  scene_collection m_scenes;
  scene_collection::iterator m_currentScene;
  bool m_paused { false };

};

template <typename T, class... Args> auto scene_controller::AddScene(Args&&... args) -> void
{
  m_scenes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
}
