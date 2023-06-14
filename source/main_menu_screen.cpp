#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_text_format_def.h"
#include "play_screen.h"
#include "global_state.h"
#include "button.h"
#include "dwrite_factory.h"
#include "render_target_area.h"

main_menu_screen::main_menu_screen()
{
  sound_data::create(framework::directSound().get(), L"data");
  
  global_sound_buffer_selector dummySelector { sound_data::soundBuffers() };
  auto soundBuffer = dummySelector[sound_buffer_name::menu_theme];
  auto volumeRange = float { DSBVOLUME_MAX - DSBVOLUME_MIN };
  auto volume = static_cast<int>(DSBVOLUME_MIN + volumeRange * 0.8f);
  HRESULT hr = soundBuffer->SetVolume(volume);
  if( FAILED(hr) ) throw std::exception();
  
  // play sound now to ensure no sound glitch on first real play
  {
    sound_buffer_player dummyPlayer(soundBuffer);
    dummyPlayer.Play();
  }

  m_menu = GetMenuDef().CreateMenu();
}

auto main_menu_screen::Refresh(const screen_input_state& inputState, int64_t ticks) -> bool
{
  framework::toggleFullScreenOnKeyPress(inputState, DIK_F12);

  Update(inputState, ticks);

  Render();

  framework::present();

  return m_continueRunning;
}

auto main_menu_screen::Update(const screen_input_state& inputState, int64_t frameInterval) -> void
{
  if( m_startPlay )
  {
    framework::openScreen<play_screen>();
    m_startPlay = false;
  }

  screen_transform screenTransform;
  auto inputData = screenTransform.GetObjectInputData(inputState);

  m_menu.Update(inputData);
  m_cursor.Update(inputData);
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
  m_cursor.Render(viewRect);
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
