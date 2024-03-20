#pragma once

#include "play_scene.h"
#include "stopwatch.h"
#include "end_of_level_menu_controller.h"

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
  {
    m_menuController.OpenRoot();
  }

  auto Begin() -> void override
  {
    SetCameraZoom(GetPlayCameraZoom());
    m_playState->LevelContainer().SetPlayerActive(false);
    m_playState->LevelContainer().SetPlayerInvulnerable(true);
  }

  auto Update(int64_t ticks) -> bool override
  {
    play_scene::Update(ticks);

    if( m_stopwatch.Expired() )
    {
      m_menuController.Update();
    }

    return m_stopwatch.Update(ticks) || m_menuController.Selection() == end_of_level_menu_controller::selection::NONE ? true : false;
  }

  auto Render() const -> void override
  {
    play_scene::Render();
    
    if( m_stopwatch.Expired() )
    {
      render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
      D2D1_SIZE_F renderTargetSize = render_target::get()->GetSize();
      m_menuController.Render(D2D1_RECT_F { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1});
    }
  }

private:

  stopwatch m_stopwatch { 3.0f };
  end_of_level_menu_controller m_menuController;

};
