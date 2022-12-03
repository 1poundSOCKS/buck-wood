#ifndef _main_menu_screen_
#define _main_menu_screen_

#include "framework.h"
#include "control_state.h"
#include "diagnostics.h"
#include "game_objects.h"
#include "sound_buffer_selector.h"
#include "screen_render.h"

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
  main_menu_screen_state();

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  float renderTargetMouseX = 0, renderTargetMouseY = 0;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
  bool saveGameLevelData = false;
  bool checkSaveOnExit = true;
};

void RefreshControlState(main_menu_control_state& screenControlState, const control_state& controlState);

void UpdateScreenState(main_menu_screen_state& screenState, const main_menu_control_state& controlState, const system_timer& timer);

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const main_menu_screen_state& screenState);

void PlaySoundEffects(const main_menu_screen_state& screenState, global_sound_buffer_selector_type soundBuffers);

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const main_menu_screen_state& screenState, const main_menu_control_state& controlState);

bool ContinueRunning(const main_menu_screen_state& screenState);

#endif
