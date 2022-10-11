#include "render.h"
#include <string>

const float titleScreenWidth = 1000.0f;
const float titleScreenHeight = 500.0f;

void RenderFrame(const d2d_frame& frame, 
                 const game_state& gameState, 
                 const perf_data& pd)
{
  const bool renderDiagnostics = true;

  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  switch( gameState.screen )
  {
    case game_state::main:
      RenderMainScreen(frame, gameState);
      if( renderDiagnostics ) RenderDiagnostics(frame, gameState, pd);
      break;
    case game_state::title:
      RenderTitleScreen(frame);
      break;
  }
}

void RenderMainScreen(const d2d_frame& frame, const game_state& gameState)
{
  std::unique_ptr<D2D1::Matrix3x2F> scaleTransform = CreateScaleTransform(frame, gameState.currentLevel->width, gameState.currentLevel->height);

  DrawLevel(*gameState.currentLevel, frame);

  DrawPlayer(*gameState.player, frame, *scaleTransform);

  for( const std::unique_ptr<bullet>& bullet : gameState.bullets )
  {
    DrawBullet(*bullet, frame, *scaleTransform);
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
  // frame.renderTarget->DrawTextW(msg.c_str(), msg.length(), frame.writeTextFormat.get(), rect, frame.brush.get());

  rect = D2D1::RectF(0, 0, size.width / 4, size.height / 4);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  for( const auto& text: pd.additionalInfo )
  {
    msg += text;
    msg += L"\n";
  }

  frame.renderTarget->DrawTextW(msg.c_str(),msg.length(), frame.writeTextFormat.get(), rect, frame.brush.get());
}

void DrawPlayer(const player_ship& player, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(player.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(player.xPos, player.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * scaleTransform;// * frame.transform;
  frame.renderTarget->SetTransform(transform);
  DrawShape(*player.outline, frame);
}

void DrawBullet(const bullet& bullet, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform)
{
  static const float bulletSize = 3.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  const D2D1::Matrix3x2F transform = translate * scaleTransform;// * frame.transform;
  frame.renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  frame.renderTarget->FillRectangle(&rectangle, frame.brush.get());
}

void DrawLevel(const game_level& level, const d2d_frame& frame)
{
  std::unique_ptr<D2D1::Matrix3x2F> scaleTransform = CreateScaleTransform(frame, level.width, level.height);
  D2D1::Matrix3x2F transform = *scaleTransform;// * frame.transform;
  frame.renderTarget->SetTransform(transform);
  DrawShape(*level.boundary, frame);

  for( const auto& shape: level.objects )
  {
    DrawShape(*shape, frame);
  }
}

void DrawShape(const game_shape& shape, const d2d_frame& frame)
{
  for( const auto& line : shape.lines )
  {
    D2D1_POINT_2F startPoint;
    startPoint.x = line.first.x;
    startPoint.y = line.first.y;

    D2D1_POINT_2F endPoint;
    endPoint.x = line.second.x;
    endPoint.y = line.second.y;

    frame.renderTarget->DrawLine(startPoint, endPoint, frame.brush.get());
  }
}

std::unique_ptr<D2D1::Matrix3x2F> CreateScaleTransform(const d2d_frame& frame, float screenWidth, float screenHeight)
{
  D2D1_SIZE_F frameSize = frame.renderTarget->GetSize();
  D2D1_SIZE_F size;
  size.width = frameSize.width / screenWidth;
  size.height = frameSize.height / screenHeight;
  return std::make_unique<D2D1::Matrix3x2F>(D2D1::Matrix3x2F::Scale(size));
}
