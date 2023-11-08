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
#include "settings_menu_def.h"

main_menu_screen::main_menu_screen()
{
  sound_data::create(framework::directSound().get(), L"data");
  
  auto musicBuffers = std::array
  {
    sound_data::get(sound_data::menu_theme)
  };

  volume_controller musicVolumeController { musicBuffers };
  musicVolumeController.SetVolume(0.8f);

  auto effectBuffers = std::array
  {
    sound_data::get(sound_data::shoot),
    sound_data::get(sound_data::thrust),
    sound_data::get(sound_data::target_activated),
    sound_data::get(sound_data::mine_exploded)
  };
  
  volume_controller effectVolumeController { effectBuffers };
  effectVolumeController.SetVolume(0.8f);
  
  // play sound now to ensure no sound glitch on first real play
  {
    sound_buffer_player player { sound_data::get(sound_data::menu_theme) };
    player.Play();
  }

  const auto& renderTarget = framework::renderTarget();
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
    framework::openScreen<play_screen>();
    m_startPlay = false;
  }
  else
  {
    menu_control_data menuControlData { framework::screenInputState() };
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
  const auto& renderTarget = framework::renderTarget();

  render_guard renderGuard { renderTarget };
  
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  screen_transform screenTransform;
  renderTarget->SetTransform(screenTransform.Get());
  auto viewRect = screenTransform.GetViewRect(renderTarget->GetSize());

  m_menuController.Render(viewRect);
}
