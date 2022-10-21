#include "render.h"
#include <string>
#include "level_editor.h"

const float titleScreenWidth = 1000.0f;
const float titleScreenHeight = 500.0f;

void RenderFrame(const d2d_frame& frame, game_state& gameState)
{
  const bool renderDiagnostics = true;

  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  switch( gameState.screen )
  {
    case game_state::screen_play:
      RenderPlayScreen(frame, gameState);
      break;
    case game_state::screen_title:
      RenderTitleScreen(frame);
      break;
    case game_state::screen_level_editor:
      RenderLevelEditor(frame, gameState);
      break;
  }
}

void RenderPlayScreen(const d2d_frame& frame, game_state& gameState)
{
  auto& playState = *gameState.playState;
  auto& currentLevel = *playState.currentLevel;

  D2D1::Matrix3x2F levelTransform = CreateLevelTransform(frame.renderTarget, playState);

  RenderLevel(currentLevel, frame, levelTransform);

  RenderPlayer(*playState.player, frame, levelTransform);

  for( const std::unique_ptr<bullet>& bullet : playState.bullets )
  {
    RenderBullet(*bullet, frame, levelTransform);
  }

  if( playState.state == play_state::complete )
  {
    std::wstring text = L"F*CK YEAH";
    D2D_SIZE_F size = frame.renderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
    frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
  }
  else if( playState.playerState == play_state::player_dead )
  {
    std::wstring text = L"YOU LOSE";
    D2D_SIZE_F size = frame.renderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
    frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.textFormats->levelEndTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
  }

  float levelTimerInSeconds = playState.levelTimerStop == 0 ? 
    GetElapsedTimeInSeconds(playState.levelTimerStart, playState.totalTicks, playState.ticksPerSecond) :
    GetElapsedTimeInSeconds(playState.levelTimerStart, playState.levelTimerStop, playState.ticksPerSecond);

  RenderTimer(frame, levelTimerInSeconds);

  if( levelTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = gameState.renderTargetMouseX;
    inPoint.y = gameState.renderTargetMouseY;
    D2D1_POINT_2F outPoint = levelTransform.TransformPoint(inPoint);
    playState.levelMouseX = outPoint.x;
    playState.levelMouseY = outPoint.y;
  }
}

void RenderTitleScreen(const d2d_frame& frame)
{
  std::wstring titleText = L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";
  titleText += L"\nPress F1 for level editor";

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(titleText.c_str(),titleText.length(), frame.textFormats->menuTextFormat.get(), rect, frame.brushes->brushLevelEndText.get());
}

void RenderLevelEditor(const d2d_frame& frame, game_state& gameState)
{
  auto& levelEditorState = *gameState.levelEditorState;
  auto& currentLevel = *levelEditorState.level;

  D2D1::Matrix3x2F levelTransform = D2D1::Matrix3x2F::Translation(levelEditorState.viewX, levelEditorState.viewY);
  RenderLevel(currentLevel, frame, levelTransform);

  if( levelTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = gameState.renderTargetMouseX;
    inPoint.y = gameState.renderTargetMouseY;
    D2D1_POINT_2F outPoint = levelTransform.TransformPoint(inPoint);
    levelEditorState.levelMouseX = outPoint.x;
    levelEditorState.levelMouseY = outPoint.y;
  }
}

void RenderDiagnostics(const d2d_frame& frame, const std::list<std::wstring>& diagnostics)
{
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);

  rect = D2D1::RectF(0, 0, size.width / 8, size.height / 6);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  std::wstring msg;
  for( const auto& text: diagnostics )
  {
    msg += text;
    msg += L"\n";
  }

  frame.renderTarget->DrawTextW(msg.c_str(),msg.length(), frame.textFormats->writeTextFormat.get(), rect, frame.brushes->brushDiagnostics.get());
}

void RenderTimer(const d2d_frame& frame, float seconds)
{
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(size.width * 7 / 8, size.height / 16, size.width - 1, size.height * 3 / 16);

  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  static wchar_t timeText[64];
  swprintf(timeText, L"%.2f", seconds);
  frame.renderTarget->DrawTextW(timeText,wcslen(timeText), frame.textFormats->levelTimerTextFormat.get(), rect, frame.brushes->brushTimer.get());
}

void RenderMouseCursor(const d2d_frame& frame, const mouse_cursor& mouseCursor)
{
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(mouseCursor.xPos, mouseCursor.yPos);
  frame.renderTarget->SetTransform(translate);
  RenderLines(mouseCursor.lines, frame.renderTarget, frame.brushes->brush);
}

void RenderPlayer(const player_ship& player, const d2d_frame& frame, const D2D1::Matrix3x2F& viewTransform)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(player.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(player.xPos, player.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * viewTransform;
  frame.renderTarget->SetTransform(transform);
  RenderShape(*player.outline, frame.renderTarget, frame.brushes->brush);

  if( player.thrusterOn )
  {
    D2D1_POINT_2F startPoint;
    startPoint.x = player.thruster->start.x;
    startPoint.y = player.thruster->start.y;

    D2D1_POINT_2F endPoint;
    endPoint.x = player.thruster->end.x;
    endPoint.y = player.thruster->end.y;

    frame.renderTarget->DrawLine(startPoint, endPoint, frame.brushes->brushThrusters.get(), 6.0f);
  }
}

void RenderBullet(const bullet& bullet, const d2d_frame& frame, const D2D1::Matrix3x2F& viewTransform)
{
  static const float bulletSize = 3.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  const D2D1::Matrix3x2F transform = translate * viewTransform;
  frame.renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  frame.renderTarget->FillRectangle(&rectangle, frame.brushes->brush.get());
}

void RenderLevel(const game_level& level, const d2d_frame& frame, const D2D1::Matrix3x2F& viewTransform)
{
  frame.renderTarget->SetTransform(viewTransform);
  RenderShape(*level.boundary, frame.renderTarget, frame.brushes->brush);

  for( const auto& shape: level.objects )
  {
    RenderShape(*shape, frame.renderTarget, frame.brushes->brush);
  }

  for( const auto& target: level.targets)
  {
    const winrt::com_ptr<ID2D1SolidColorBrush>& targetBrush = target->state == 
      target::ACTIVATED ? frame.brushes->brushActivated : frame.brushes->brushDeactivated;
    
    RenderShape(target->shape, frame.renderTarget, targetBrush);
  }
}

void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  RenderLines(shape.lines, renderTarget, brush);
}

void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  for( const auto& line : lines )
  {
    D2D1_POINT_2F startPoint;
    startPoint.x = line.start.x;
    startPoint.y = line.start.y;

    D2D1_POINT_2F endPoint;
    endPoint.x = line.end.x;
    endPoint.y = line.end.y;

    renderTarget->DrawLine(startPoint, endPoint, brush.get(), 2.0f);
  }
}

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const game_state& gameState)
{
  if( gameState.screen != game_state::screen_play ) return D2D1::Matrix3x2F::Identity();

  return CreateLevelTransform(renderTarget, *gameState.playState);
}

D2D1::Matrix3x2F CreateLevelTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const play_state& playState)
{
  float levelWidth = playState.currentLevel->width;
  float playerPosY = playState.player->yPos;

  D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

  float scale = renderTargetSize.width / levelWidth;
  float shiftY = renderTargetSize.height / 2 * scale;
  
  D2D1::Matrix3x2F matrixShift = D2D1::Matrix3x2F::Translation(0, shiftY - playerPosY);
  
  D2D1_SIZE_F scaleSize;
  scaleSize.width = scale;
  scaleSize.height = scale;
  D2D1::Matrix3x2F matrixScale = D2D1::Matrix3x2F::Scale(scaleSize);

  return matrixScale * matrixShift;
}
