#include "pch.h"
#include "framework.h"

#include "main_menu_screen.h"
#include "play_screen.h"
#include "screen_container.h"
#include "game_settings.h"

main_menu_screen::main_menu_screen()
{
  m_menuController.OpenRoot();
}

auto main_menu_screen::Refresh(int64_t ticks) -> bool
{
  Render();
  return Update(ticks);
}

auto main_menu_screen::Update(int64_t frameInterval) -> bool
{
  bool continueRunning = true;

  m_menuController.Update();

  switch( m_menuController.Selection() )
  {
    case main_menu_controller::selection::start:
      StartPlay();
      break;
    case main_menu_controller::selection::exit:
      continueRunning = false;
      break;
  }

  return continueRunning;
}

auto main_menu_screen::Render() const -> void
{
  render_guard renderGuard { render_target::get() };
  render_target::get()->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
  auto renderTargetSize = render_target::get()->GetSize();
  m_menuController.Render(D2D1_RECT_F { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1 });
}

auto main_menu_screen::StartPlay() -> void
{
  screen_container<play_screen> playScreen { game_settings::framerate(), DIK_F12 };
  windows_message_loop::run(playScreen);
}
