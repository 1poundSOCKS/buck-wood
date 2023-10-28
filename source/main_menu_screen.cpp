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
#include "options_menu_def.h"

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

  m_menuController.Open(GetMenuDef(menu_id::root));
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
    m_menuController.GetCurrent().Update(menuControlData);
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

  m_menuController.GetCurrent().Render(viewRect);
}

[[nodiscard]] auto main_menu_screen::GetMenuDef(menu_id id) -> menu_def
{
  const auto& renderTarget = framework::renderTarget();

  auto menuArea = render_target_area(renderTarget->GetSize(), render_target_area::contraint_centred(0.4f, 0.4f));

  menu_def menuDef(menuArea.GetRect());

  switch( id )
  {

  case menu_id::root:

    menuDef.AddButtonDef({ L"Start", [this]() -> void
    {
      m_startPlay = true;
    }});

    menuDef.AddButtonDef({ L"Options", [this]() -> void
    {
      m_menuController.Open(GetMenuDef(menu_id::options));
    }});

    menuDef.AddButtonDef({ L"Exit", [this]() -> void
    {
      m_continueRunning = false;
    }});

    break;

  case menu_id::options:

    menuDef = GetOptionsMenuDef(menuArea, [this]() -> void
    {
      m_menuController.Close();
    });

    break;

  default:
    break;

  }

  menuDef.UpdateButtons();

  return menuDef;
}
