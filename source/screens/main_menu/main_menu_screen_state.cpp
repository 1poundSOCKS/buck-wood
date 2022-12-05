#include "pch.h"
#include "main_menu_screen_state.h"
#include "input_state.h"
#include "timers.h"

void UpdateScreenExitState(main_menu_screen_state& screenState, const screen_input_state& screenInputState);

void UpdateScreenState(main_menu_screen_state& screenState, const screen_input_state& screenInputState, const system_timer& timer)
{
  screenState.mousePointerX = screenInputState.windowData.mouse.x;
  screenState.mousePointerY = screenInputState.windowData.mouse.y;
  
  if( screenState.viewState == main_menu_screen_state::view_exit )
  {
    UpdateScreenExitState(screenState, screenInputState);
    return;
  }

  if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    if( screenState.checkSaveOnExit ) screenState.viewState = main_menu_screen_state::view_exit;
    else screenState.quit = true;
    return;
  }

  screenState.renderTargetMouseData = screenInputState.renderTargetMouseData;
  
  screenState.startPlay = screenState.startLevelEdit = false;
  
  if( screenState.starting )
  {
    screenState.starting = false;
    return;
  }

  if( KeyPressed(screenInputState, DIK_SPACE) )
  {
    screenState.startPlay = true;
    return;
  }

  if( KeyPressed(screenInputState, DIK_F1) )
  {
    screenState.startLevelEdit = true;
    return;
  }
}

void UpdateScreenExitState(main_menu_screen_state& screenState, const screen_input_state& screenInputState)
{
  if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    screenState.viewState = main_menu_screen_state::view_default;
  }
  else if( KeyPressed(screenInputState, DIK_N) )
  {
    screenState.saveGameLevelData = false;
    screenState.quit = true;
  }
  else if( KeyPressed(screenInputState, DIK_Y) )
  {
    screenState.saveGameLevelData = true;
    screenState.quit = true;
  }
}

bool ContinueRunning(const main_menu_screen_state& screenState)
{
  return screenState.quit || screenState.startPlay || screenState.startLevelEdit ? false : true;
}
