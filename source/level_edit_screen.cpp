#include "level_edit_screen.h"
#include "render.h"

level_edit_screen_state::level_edit_screen_state(const global_state& globalState) : gameLevelDataIndex(*globalState.gameLevelDataIndex)
{
  currentLevelDataIterator = gameLevelDataIndex.begin();
  const auto& levelData = *currentLevelDataIterator;
  currentLevel = std::make_unique<game_level>(*levelData);
}

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const level_edit_screen_state& screenState)
{
  auto renderTargetSize = renderTarget->GetSize();
  return CreateGameLevelTransform(0, 0, 0.3f, renderTargetSize.width, renderTargetSize.height);
}

void RenderFrame(const d2d_frame& frame, level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderLevel(frame, *screenState.currentLevel);

  RenderMouseCursor(frame, screenState.mouseCursor);

  auto levelTransform = frame.viewTransform;
  if( levelTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = frame.renderTargetMouseX;
    inPoint.y = frame.renderTargetMouseY;
    auto outPoint = levelTransform.TransformPoint(inPoint);
    screenState.levelMouseX = outPoint.x;
    screenState.levelMouseY = outPoint.y;
  }
}

void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.returnToMenu = false;

  if( controlState.quitPress ) screenState.returnToMenu = true;
}

void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state)
{
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnosticsData, controlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"level mouse X: %.2f\n", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"level mouse Y: %.2f\n", screenState.levelMouseY);
  diagnosticsData.push_back(text);
}
