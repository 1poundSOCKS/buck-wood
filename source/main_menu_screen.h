#ifndef _main_menu_screen_
#define _main_menu_screen_

#include "framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "diagnostics.h"
#include "game_objects.h"
#include "global_state.h"
#include "screens.h"

struct main_menu_control_state
{
  bool quit = false;
  bool startPlay = false;
  bool startLevelEditor = false;
  bool cancelExit = false;
  bool saveChanges = false;
  bool discardChanges = false;
  float renderTargetMouseX = 0;
  float renderTargetMouseY = 0;
};

struct main_menu_screen_state
{
  main_menu_screen_state(const d2d_app& app, const global_state& globalState);

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  const global_state& globalState;
  const render_brushes renderBrushes;
  const render_text_formats& textFormats;

  float renderTargetMouseX = 0, renderTargetMouseY = 0;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
  bool saveGameLevelData = false;
  bool checkSaveOnExit = true;

  sound_buffer_player musicPlayer;
};

void RefreshControlState(main_menu_control_state& screenControlState, const control_state& controlState);
void UpdateScreenState(main_menu_screen_state& screenState, const main_menu_control_state& controlState, const system_timer& timer);
void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState);
void PlaySoundEffects(const main_menu_screen_state& screenState);
void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const main_menu_screen_state& screenState, const main_menu_control_state& controlState);
screen_status GetScreenStatus(const main_menu_screen_state& screenState);
void UpdateGlobalState(global_state& globalState, const main_menu_screen_state& screenState);

#endif
