#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_text_format_def.h"
#include "play_screen.h"
#include "global_state.h"
#include "button.h"
#include "dwrite_factory.h"
#include "render_target_area.h"
#include "sound_buffer_player.h"
#include "volume_controller.h"
#include "game_settings.h"
#include "game_volume_controller.h"

main_menu_screen::main_menu_screen()
{
  game_settings::load();
  
  sound_data::create(render_target::directSound().get(), L"data");

  game_volume_controller::create();
  game_volume_controller::setEffectsVolume(6);
  game_volume_controller::setMusicVolume(7);

  // play sound now to ensure no sound glitch on first real play
  {
    sound_buffer_player player { sound_data::get(sound_data::menu_theme) };
    player.Play();
  }

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
    render_target::openScreen<play_screen>();
    m_startPlay = false;
  }
  else
  {
    menu_control_data menuControlData { render_target::screenInputState() };
    m_menuController.Update(menuControlData);

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
