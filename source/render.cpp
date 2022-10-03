#include "render.h"
#include <string>

const float titleScreenWidth = 1000.0f;
const float titleScreenHeight = 500.0f;

void DoRender(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, 
              const std::unique_ptr<game_state>& gameState, 
              const std::unique_ptr<perf_data>& pd, 
              float mouseX, float mouseY)
{
  const bool renderDiagnostics = false;

  std::unique_ptr<d2d_frame> frame = std::make_unique<d2d_frame>(renderTarget);
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  
  switch( gameState->screen )
  {
    case game_state::main:
      RenderMainScreen(frame, gameState, mouseX, mouseY);
      if( renderDiagnostics ) RenderDiagnostics(frame, gameState, pd);
      break;
    case game_state::title:
      RenderTitleScreen(frame);
      break;
  }
}

void RenderMainScreen(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, float mouseX, float mouseY)
{
  std::unique_ptr<D2D1::Matrix3x2F> scaleTransform = CreateScaleTransform(frame->renderTarget, gs->currentLevel->width, gs->currentLevel->height);

  D2D1_SIZE_F frameSize = frame->renderTarget->GetSize();
  gs->cursor.xPos = mouseX * gs->currentLevel->width / frameSize.width;
  gs->cursor.yPos = mouseY * gs->currentLevel->height / frameSize.height;

  DrawLevel(*gs->currentLevel, *frame);

  DrawGameObject(gs->player, frame, scaleTransform);

  for( const std::unique_ptr<bullet>& bullet : gs->bullets )
  {
    DrawGameObject(bullet->gameObject, frame, scaleTransform);
  }
}

void RenderTitleScreen(const std::unique_ptr<d2d_frame>& frame)
{
  std::wstring titleText = L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  D2D_SIZE_F size = frame->renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  frame->renderTarget->DrawTextW(titleText.c_str(),titleText.length(),frame->writeTextFormat.get(),rect,frame->brush.get());
}

void RenderDiagnostics(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd)
{
  wchar_t fps[32];
  _ui64tow(pd->fps, fps, 10);

  wchar_t bulletCount[32];
  wsprintf(bulletCount, L"%i", gs->bullets.size());

  std::wstring msg = std::wstring(fps) + std::wstring(L"\n") + bulletCount;
  D2D_SIZE_F size = frame->renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame->renderTarget->DrawTextW(msg.c_str(),msg.length(),frame->writeTextFormat.get(),rect,frame->brush.get());
}

void DrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<D2D1::Matrix3x2F>& scaleTransform)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(gameObject.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(gameObject.xPos, gameObject.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * (*scaleTransform);
  frame->renderTarget->SetTransform(transform);
  DrawGameObject(gameObject, frame);
}

void DrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame)
{
  D2D1_RECT_F rectangle = D2D1::RectF(- gameObject.size / 2, - gameObject.size / 2, gameObject.size / 2, gameObject.size / 2);
  frame->renderTarget->FillRectangle(&rectangle, frame->brush.get());
}

void DrawLevel(const game_level& level, const d2d_frame& frame)
{
  
}

std::unique_ptr<D2D1::Matrix3x2F> CreateScaleTransform(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, float screenWidth, float screenHeight)
{
  D2D1_SIZE_F frameSize = renderTarget->GetSize();
  D2D1_SIZE_F size;
  size.width = frameSize.width / screenWidth;
  size.height = frameSize.height / screenHeight;
  return std::make_unique<D2D1::Matrix3x2F>(D2D1::Matrix3x2F::Scale(size));
}
