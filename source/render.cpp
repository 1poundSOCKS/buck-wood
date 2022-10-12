#include "render.h"
#include <string>

const float titleScreenWidth = 1000.0f;
const float titleScreenHeight = 500.0f;

void RenderFrame(const d2d_frame& frame, const game_state& gameState, const D2D1::Matrix3x2F& transform)
{
  const bool renderDiagnostics = true;

  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  switch( gameState.screen )
  {
    case game_state::main:
      RenderMainScreen(frame, gameState, transform);
      break;
    case game_state::title:
      RenderTitleScreen(frame);
      break;
  }
}

void RenderMainScreen(const d2d_frame& frame, const game_state& gameState, const D2D1::Matrix3x2F& transform)
{
  DrawLevel(*gameState.currentLevel, frame, transform);

  DrawPlayer(*gameState.player, frame, transform);

  for( const std::unique_ptr<bullet>& bullet : gameState.bullets )
  {
    DrawBullet(*bullet, frame, transform);
  }

  if( gameState.playerState == game_state::dead )
  {
    std::wstring text = L"game over";

    D2D_SIZE_F size = frame.renderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
    frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    frame.renderTarget->DrawTextW(text.c_str(),text.length(), frame.writeTextFormat.get(), rect, frame.brush.get());
  }
}

void RenderTitleScreen(const d2d_frame& frame)
{
  std::wstring titleText = L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame.renderTarget->DrawTextW(titleText.c_str(),titleText.length(), frame.writeTextFormat.get(), rect, frame.brush.get());
}

void RenderDiagnostics(const d2d_frame& frame, const game_state& gameState, const perf_data& pd)
{
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  wchar_t fps[32];
  _ui64tow(pd.fps, fps, 10);

  wchar_t bulletCount[32];
  wsprintf(bulletCount, L"bullet count: %i", gameState.bullets.size());

  static const std::wstring eol = std::wstring(L"\n");

  std::wstring msg = std::wstring(L"fps: ") + std::wstring(fps);
  msg += eol;
  msg += std::wstring(bulletCount);
  msg += eol;

  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);

  rect = D2D1::RectF(0, 0, size.width / 4, size.height / 4);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  for( const auto& text: pd.additionalInfo )
  {
    msg += text;
    msg += L"\n";
  }

  frame.renderTarget->DrawTextW(msg.c_str(),msg.length(), frame.writeTextFormat.get(), rect, frame.brush.get());
}

void RenderMouseCursor(const d2d_frame& frame, const mouse_cursor& mouseCursor)
{
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(mouseCursor.xPos, mouseCursor.yPos);
  frame.renderTarget->SetTransform(translate);
  DrawLines(mouseCursor.lines, frame.renderTarget, frame.brush);
}

void DrawPlayer(const player_ship& player, const d2d_frame& frame, const D2D1::Matrix3x2F& viewTransform)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(player.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(player.xPos, player.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * viewTransform;
  frame.renderTarget->SetTransform(transform);
  DrawShape(*player.outline, frame.renderTarget, frame.brush);
}

void DrawBullet(const bullet& bullet, const d2d_frame& frame, const D2D1::Matrix3x2F& viewTransform)
{
  static const float bulletSize = 3.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  const D2D1::Matrix3x2F transform = translate * viewTransform;
  frame.renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  frame.renderTarget->FillRectangle(&rectangle, frame.brush.get());
}

void DrawLevel(const game_level& level, const d2d_frame& frame, const D2D1::Matrix3x2F& viewTransform)
{
  frame.renderTarget->SetTransform(viewTransform);
  DrawShape(*level.boundary, frame.renderTarget, frame.brush);

  for( const auto& shape: level.objects )
  {
    DrawShape(*shape, frame.renderTarget, frame.brush);
  }

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(level.target->x, level.target->y);
  frame.renderTarget->SetTransform(translate * viewTransform);
  DrawShape(level.target->shape, frame.renderTarget, frame.brushDeactivated);
}

void DrawShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  DrawLines(shape.lines, renderTarget, brush);
}

void DrawLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
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

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float widthToScale, float heightToTranslate)
{
  D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();
  
  D2D1::Matrix3x2F shift = D2D1::Matrix3x2F::Translation(0, heightToTranslate);
  
  D2D1_SIZE_F scaleSize;
  scaleSize.width = renderTargetSize.width / widthToScale;
  scaleSize.height = scaleSize.width;
  D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(scaleSize);

  return scale * shift;
}
