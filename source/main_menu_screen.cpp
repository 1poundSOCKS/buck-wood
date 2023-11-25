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
#include "render_screen.h"

main_menu_screen::main_menu_screen()
{
  const auto& renderTarget = render_target::renderTarget();
  auto menuArea = render_target_area(renderTarget->GetSize(), render_target_area::contraint_centred(0.4f, 0.4f));
  m_menuController.OpenRoot(menuArea);
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
    render_screen<play_screen> playScreen { 60, DIK_F12 };
    windows_message_loop::run(playScreen);
    m_startPlay = false;
  }
  else
  {
    m_menuController.Update(menu_control_data { user_input::keyboardReader(), user_input::gamepadReader() });

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
  const auto& renderTarget = render_target::renderTarget();

  render_guard renderGuard { renderTarget };
  
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  screen_transform screenTransform;
  renderTarget->SetTransform(screenTransform.Get());
  auto viewRect = screenTransform.GetViewRect(renderTarget->GetSize());

  m_menuController.Render(viewRect);
}
