#include "render.h"
#include <string>

void DoRender(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd)
{
  const bool renderDiagnostics = false;

  frame->renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  frame->renderTarget->SetTransform(*frame->scale.get());
  
  switch( gs->screen )
  {
    case game_state::main:
      RenderMainScreen(frame, gs);
      if( renderDiagnostics ) RenderDiagnostics(frame, gs, pd);
      break;
    case game_state::title:
      RenderTitleScreen(frame);
      break;
  }
}

void RenderMainScreen(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs)
{

  D2D1_SIZE_F renderTargetSize = frame->renderTarget->GetSize();

  SetTransformAndDrawGameObject(gs->player, frame);

  for( const std::unique_ptr<bullet>& bullet : gs->bullets )
  {
    SetTransformAndDrawGameObject(bullet->gameObject, frame);
  }

  SetTransformAndDrawGameObject(gs->cursor, frame);
}

void RenderDiagnostics(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd)
{
  wchar_t fps[32];
  _ui64tow(pd->fps, fps, 10);

  wchar_t bulletCount[32];
  wsprintf(bulletCount, L"%i", gs->bullets.size());

  std::wstring msg = std::wstring(fps) + std::wstring(L"\n") + bulletCount;
  D2D1_RECT_F gameRect = D2D1::RectF(0, 0, frame->gameScreenWidth - 1, frame->gameScreenHeight - 1);
  frame->renderTarget->DrawTextW(msg.c_str(),msg.length(),frame->writeTextFormat.get(),gameRect,frame->brush.get());
}

void RenderTitleScreen(const std::unique_ptr<d2d_frame>& frame)
{
  std::wstring titleText = L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";

  D2D1_RECT_F gameRect = D2D1::RectF(0, 0, frame->gameScreenWidth - 1, frame->gameScreenHeight - 1);
  frame->renderTarget->DrawTextW(titleText.c_str(),titleText.length(),frame->writeTextFormat.get(),gameRect,frame->brush.get());
}

void SetTransformAndDrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(gameObject.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(gameObject.xPos, gameObject.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * (*frame->scale.get());
  frame->renderTarget->SetTransform(transform);
  DrawGameObject(gameObject, frame);
}

void DrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame)
{
  D2D1_RECT_F rectangle = D2D1::RectF(- gameObject.size / 2, - gameObject.size / 2, gameObject.size / 2, gameObject.size / 2);
  frame->renderTarget->FillRectangle(&rectangle, frame->brush.get());
}
