#include "render.h"
#include <string>

const float titleScreenWidth = 1000.0f;
const float titleScreenHeight = 500.0f;

void DoRender(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, 
              const game_state& gameState, 
              const perf_data& pd, 
              float mouseX, float mouseY)
{
  const bool renderDiagnostics = false;

  std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(renderTarget);
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  switch( gameState.screen )
  {
    case game_state::main:
      RenderMainScreen(*frame, gameState, mouseX, mouseY);
      if( renderDiagnostics ) RenderDiagnostics(*frame, gameState, pd);
      break;
    case game_state::title:
      RenderTitleScreen(*frame);
      break;
  }
}

void RenderMainScreen(const d2d_frame& frame, const game_state& gameState, float mouseX, float mouseY)
{
  std::unique_ptr<D2D1::Matrix3x2F> scaleTransform = CreateScaleTransform(frame, gameState.currentLevel->width, gameState.currentLevel->height);

  DrawLevel(*gameState.currentLevel, frame);

  DrawGameObject(gameState.player, frame, *scaleTransform);

  for( const std::unique_ptr<bullet>& bullet : gameState.bullets )
  {
    DrawGameObject(bullet->gameObject, frame, *scaleTransform);
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
  wchar_t fps[32];
  _ui64tow(pd.fps, fps, 10);

  wchar_t bulletCount[32];
  wsprintf(bulletCount, L"%i", gameState.bullets.size());

  std::wstring msg = std::wstring(fps) + std::wstring(L"\n") + bulletCount;
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->DrawTextW(msg.c_str(), msg.length(), frame.writeTextFormat.get(), rect, frame.brush.get());
}

void DrawGameObject(const game_object& gameObject, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(gameObject.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(gameObject.xPos, gameObject.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * scaleTransform;
  frame.renderTarget->SetTransform(transform);
  DrawGameObject(gameObject, frame);
}

void DrawGameObject(const game_object& gameObject, const d2d_frame& frame)
{
  D2D1_RECT_F rectangle = D2D1::RectF(- gameObject.size / 2, - gameObject.size / 2, gameObject.size / 2, gameObject.size / 2);
  frame.renderTarget->FillRectangle(&rectangle, frame.brush.get());
}

void DrawLevel(const game_level& level, const d2d_frame& frame)
{
  std::unique_ptr<D2D1::Matrix3x2F> scaleTransform = CreateScaleTransform(frame, level.width, level.height);
  frame.renderTarget->SetTransform(*scaleTransform);

  for( const auto& line : level.boundary )
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
