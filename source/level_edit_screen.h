#ifndef _level_edit_screen_
#define _level_edit_screen_

#include "screens/level_edit/level_edit_screen_state.h"
#include "timers.h"

void UpdateScreenState
(
  level_edit_screen_state& screenState, 
  const screen_input_state& inputState
);

void RenderFrame
(
  ID2D1RenderTarget* renderTarget, 
  const level_edit_screen_state& screenState
);

void PlaySoundEffects
(
  const level_edit_screen_state& screenState
);

bool ContinueRunning(const level_edit_screen_state& screenState);

void FormatDiagnostics(const level_edit_screen_state& screenState, auto diagnosticsDataInserter)
{
  diagnosticsDataInserter = std::format(L"mouse: {}, {}", screenState.levelMouseX, screenState.levelMouseY);
}

#endif
