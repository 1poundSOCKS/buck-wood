#include "pch.h"
#include "framework.h"

#include "main_menu_screen.h"
#include "play_screen.h"

#include "render_text_format_def.h"
#include "global_state.h"
#include "button.h"
#include "render_target_area.h"
#include "sound_buffer_player.h"
#include "volume_controller.h"
#include "game_volume_controller.h"
#include "screen_container.h"
#include "game_settings.h"

main_menu_screen::main_menu_screen()
{
  m_menuController.OpenRoot( render_target_area { render_target::get()->GetSize(), render_target_area::contraint_centred(0.5f, 1.0f) } );
}

auto main_menu_screen::Refresh(int64_t ticks) -> bool
{
  Update(ticks);
  Render();
  return m_continueRunning;
}

auto main_menu_screen::Update(int64_t frameInterval) -> void
{
  if( m_startPlay )
  {
    screen_container<play_screen> playScreen { game_settings::framerate(), DIK_F12 };
    windows_message_loop::run(playScreen);
    m_startPlay = false;
  }
  else
  {
    m_menuController.Update();

    switch( m_menuController.Selection() )
    {
      case main_menu_controller::selection::start:
        m_startPlay = true;
        break;
      case main_menu_controller::selection::exit:
        m_continueRunning = false;
        break;
    }
  }
}

auto main_menu_screen::Render() const -> void
{
  render_guard renderGuard { render_target::get() };
  
  render_target::get()->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  screen_transform screenTransform;
  render_target::get()->SetTransform(screenTransform.Get());
  auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());

  m_menuController.Render(viewRect);
}
