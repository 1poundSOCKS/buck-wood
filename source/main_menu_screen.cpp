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

main_menu_screen::main_menu_screen()
{
  sound_data::create(framework::directSound().get(), L"data");
  
  global_sound_buffer_selector soundBufferSelector { sound_data::soundBuffers() };
  
  auto menuTheme = soundBufferSelector[sound_buffer_name::menu_theme];
  auto shootEffect = soundBufferSelector[sound_buffer_name::shoot];
  auto thrustEffect = soundBufferSelector[sound_buffer_name::thrust];
  auto targetActivatedEffected = soundBufferSelector[sound_buffer_name::target_activated];
  auto mineExplodedEffect = soundBufferSelector[sound_buffer_name::mine_exploded];

  if( menuTheme )
  {
    auto musicBuffers = std::array { menuTheme };
    volume_controller musicVolumeController { musicBuffers };
    musicVolumeController.SetVolume(0.8f);

    auto effectBuffers = std::array { shootEffect, thrustEffect, targetActivatedEffected, mineExplodedEffect };
    volume_controller effectVolumeController { effectBuffers };
    effectVolumeController.SetVolume(0.8f);
  }
  
  // play sound now to ensure no sound glitch on first real play
  {
    sound_buffer_player player(menuTheme);
    player.Play();
  }

  m_menu = GetMenuDef().CreateMenu();
  m_menu.SelectFirstButton();
}

auto main_menu_screen::Refresh(int64_t ticks) -> bool
{
  framework::toggleFullscreenOnKeypress(DIK_F12);

  Update(ticks);

  Render();

  framework::present();

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
    m_menu.Update(menuControlData);
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

  m_menu.Render(viewRect);
}

[[nodiscard]] auto main_menu_screen::GetMenuDef() -> menu_def
{
  const auto& renderTarget = framework::renderTarget();

  auto menuArea = render_target_area(renderTarget->GetSize(), render_target_area::contraint_centred(0.3f, 0.3f));

  menu_def menuDef(menuArea.GetRect());

  menuDef.AddButtonDef({ L"Start", [this]() -> void
  {
    m_startPlay = true;
  }});

  menuDef.AddButtonDef({ L"Exit", [this]() -> void
  {
    m_continueRunning = false;
  }});

  menuDef.UpdateButtons();

  return menuDef;
}
