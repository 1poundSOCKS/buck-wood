#pragma once

#include "framework.h"
#include "screen_transform.h"
#include "sound_data.h"
#include "sound_data.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "play_menu_controller.h"
#include "camera_sequence.h"
#include "play_camera_transform.h"
#include "level_radar.h"
#include "player_shields.h"
#include "gamepad_thumbstick.h"

using level_container_ptr = std::shared_ptr<level_container>;

class play_scene
{

public:

  play_scene(const level_container_ptr& levelContainer) : m_levelContainer { levelContainer }
  {
  }

  virtual auto Refresh(__int64 ticks) -> bool
  {
    PlaySoundEffects();

    auto playerPosition = m_levelContainer->PlayerPosition();
    camera_sequence::camera_position cameraPosition { playerPosition.x, playerPosition.y, 1 };
    auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
    auto screenTransform = screen_transform { cameraTransform.Get() };
    auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());

    auto levelInput = ticks ? GetLevelInput() : level_input { std::nullopt, std::nullopt, 0, std::nullopt };

    m_levelContainer->Update(levelInput, ticks, viewRect);

    Render();

    return true;
  }

protected:

  auto Render() -> void
  {
    auto playerPosition = m_levelContainer->PlayerPosition();
    camera_sequence::camera_position cameraPosition { playerPosition.x, playerPosition.y, m_cameraZoom };
    auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
    auto screenTransform = screen_transform { cameraTransform.Get() };
    auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());

    render_guard renderGuard { render_target::get() };
    render_target::get()->Clear(D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));
    render_target::get()->SetTransform(screenTransform.Get());
    m_levelContainer->Render(viewRect);
  }

  [[nodiscard]] auto GetLevelInput() const -> level_input
  {
    if( gamepad_reader::connected() )
    {
      diagnostics::add(L"Left thumb X", gamepad_reader::thumb_lx());
      diagnostics::add(L"Left thumb Y", gamepad_reader::thumb_ly());
      diagnostics::add(L"Left trigger", gamepad_reader::left_trigger());

      auto rotation = gamepad_reader::thumb_lx() * 10.0f;
      auto thrust = gamepad_reader::left_trigger();
      std::optional<float> shootAngle = gamepad_reader::right_trigger() > 0 ? std::optional<float>(m_levelContainer->PlayerAngle()) : std::nullopt;

      return { std::nullopt, rotation, thrust, shootAngle };
    }
    else
    {
      return { std::nullopt, std::nullopt, 0, std::nullopt };
    }
  }

  auto PlaySoundEffects() const -> void
  {
    if( m_levelContainer->PlayerHasThrusterOn() )
    {
      sound_data::get(sound_data::thrust).Play(true);
    }
    else
    {
      sound_data::get(sound_data::thrust).Stop();
    }

    if( m_levelContainer->UpdateEvents().playerShot )
    {
      sound_data::get(sound_data::shoot).Reset();
      sound_data::get(sound_data::shoot).Play(false);
    }

    if( m_levelContainer->UpdateEvents().targetActivated )
    {
      sound_data::get(sound_data::target_activated).Reset();
      sound_data::get(sound_data::target_activated).Play(false);
    }

    if( m_levelContainer->UpdateEvents().mineExploded )
    {
      sound_data::get(sound_data::mine_exploded).Reset();
      sound_data::get(sound_data::mine_exploded).Play(false);
    }
  }

  auto SetCameraZoom(float value) -> void
  {
    m_cameraZoom = value;
  }

protected:

  level_container_ptr m_levelContainer;
  level_radar m_levelRadar;
  float m_cameraZoom { 1 };

};

class opening_play_scene : public play_scene
{

public:

  opening_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
    auto playerPosition = m_levelContainer->PlayerPosition();
    m_cameraSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 0.1f };
    m_cameraSequence.AddMove( { playerPosition.x, playerPosition.y, 1 }, performance_counter::CalculateTicks(2.0f) );
  }

  auto Refresh(__int64 ticks) -> bool override
  {
    SetCameraZoom(m_cameraSequence.GetScale(m_ticks));
    play_scene::Refresh(0);
    return ( m_ticks += ticks ) < m_cameraSequence.GetTotalTicks() ? true : false;
  }

private:

  camera_sequence m_cameraSequence;
  __int64 m_ticks { 0 };

};

class main_play_scene : public play_scene
{

public:

  main_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
    m_playerShields.Attach(m_levelContainer->PlayerShields());
  }

  auto Refresh(__int64 ticks) -> bool override
  {
    SetCameraZoom(1);
    play_scene::Refresh(ticks);

    render_guard renderGuard { render_target::get() };
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    renderer::render(m_playerShields);
    m_levelRadar.Render(m_levelContainer->PlayerPosition(), m_levelContainer->Targets());

    return m_levelContainer->HasFinished() ? false : true;
  }

private:

  player_shields m_playerShields;

};

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
  }

  auto Refresh(__int64 ticks) -> bool override
  {
    SetCameraZoom(0.5);
    play_scene::Refresh(ticks);
    return ( m_ticks -= ticks ) > 0 ? true : false;
  }

private:

  __int64 m_ticks { 20000000i64 };

};

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  [[nodiscard]] auto PausePressed() -> bool;
  auto LoadNextLevel() -> bool;

  [[nodiscard]] auto Paused() const -> bool;
  auto Pause() -> void;
  auto Unpause() -> void;
  auto Quit() -> void;

private:

  using play_scene_ptr = std::unique_ptr<play_scene>;
  using play_scene_collection = std::vector<play_scene_ptr>;

  bool m_paused = false;
  bool m_continueRunning = true;
  level_container_ptr m_levelContainer;  
  play_menu_controller m_menuController;
  game_level_data_loader m_gameLevelDataLoader;
  play_scene_collection m_scenes;
  play_scene_collection::iterator m_currentScene;

};
