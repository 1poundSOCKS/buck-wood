#include "level_edit_screen.h"
#include "render.h"
#include "game_math.h"

level_edit_screen_state::level_edit_screen_state(const global_state& globalState) : gameLevelDataIndex(*globalState.gameLevelDataIndex)
{
  currentLevelDataIterator = gameLevelDataIndex.begin();
  const auto& levelData = *currentLevelDataIterator;
  currentLevel = std::make_unique<game_level>(*levelData);
}

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const level_edit_screen_state& screenState)
{
  auto renderTargetSize = renderTarget->GetSize();
  return CreateGameLevelTransform(0, 0, 0.5f, renderTargetSize.width, renderTargetSize.height);
}

void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderLevel(frame, *screenState.currentLevel);

  if( screenState.closestPoint ) RenderHighlightedPoint(frame, *screenState.closestPoint);

  RenderMouseCursor(frame, screenState.mouseCursor);
}

void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.levelMouseX = controlState.worldMouseX;
  screenState.levelMouseY = controlState.worldMouseY;

  screenState.returnToMenu = false;

  if( controlState.quitPress ) screenState.returnToMenu = true;

  screenState.pointDistance = 100.0f;
  screenState.closestPoint = nullptr;
  for( auto& point : screenState.currentLevel->boundary->points )
  {
    float distance = GetDistanceBetweenPoints(screenState.levelMouseX, screenState.levelMouseY, point.x, point.y);
    if( distance < screenState.pointDistance )
    {
      screenState.pointDistance = distance;
      screenState.closestPoint = &point;
    }
  }

  if( screenState.closestPoint && controlState.shoot )
  {
    screenState.closestPoint->x = controlState.worldMouseX;
    screenState.closestPoint->y = controlState.worldMouseY;
    screenState.currentLevel->boundary->lines.clear();
    CreateShapeLinesFromPoints(screenState.currentLevel->boundary->lines, screenState.currentLevel->boundary->points);
  }
}

void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state)
{
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnosticsData, controlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"level mouse X: %.1f", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"level mouse Y: %.1f", screenState.levelMouseY);
  diagnosticsData.push_back(text);

  swprintf(text, L"point distance: %.1f", screenState.pointDistance);
  diagnosticsData.push_back(text);

  if( screenState.closestPoint )
  {
    swprintf(text, L"nearest point: %.1f, %.1f", screenState.closestPoint->x, screenState.closestPoint->y);
    diagnosticsData.push_back(text);
  }
}
