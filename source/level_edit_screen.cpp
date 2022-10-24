#include "level_edit_screen.h"
#include "render.h"

level_edit_screen_state::level_edit_screen_state(const game_level_data_index_ptr& gameLevelDataIndex)
{
  currentLevelDataIterator = gameLevelDataIndex->begin();
  auto levelData = *currentLevelDataIterator;
  currentLevel = std::make_shared<game_level>(*levelData);
}

void RenderFrame(const d2d_frame& frame, level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  auto renderTargetSize = frame.renderTarget->GetSize();
  float shiftX = renderTargetSize.width / 2;
  float shiftY = renderTargetSize.height / 2;
  
  auto levelTransform = D2D1::Matrix3x2F::Scale(0.3f, 0.3f) * D2D1::Matrix3x2F::Translation(shiftX, shiftY);

  RenderLevel(*screenState.currentLevel, frame, levelTransform);
}

void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.returnToMenu = false;

  if( controlState.quitPress ) screenState.returnToMenu = true;
}

void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state)
{
}

void FormatDiagnostics(std::list<std::wstring>& diagnostics, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
}
