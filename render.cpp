#include "render.h"

void DoRender(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd)
{
  frame->renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  frame->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  
  switch( gs->screen )
  {
    case game_state::main:
      RenderMainScreen(frame,gs,pd);
      break;
    case game_state::title:
      RenderTitleScreen(frame);
      break;
  }
}

void RenderMainScreen(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd)
{
  D2D1_SIZE_F renderTargetSize = frame->renderTarget->GetSize();

  SetTransformAndDrawGameObject(gs->player, frame->renderTarget, frame->brush);

  for( const std::unique_ptr<bullet>& bullet : gs->bullets )
  {
    SetTransformAndDrawGameObject(bullet->gameObject, frame->renderTarget, frame->brush);
  }

  SetTransformAndDrawGameObject(gs->cursor, frame->renderTarget, frame->brush);

  frame->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  WCHAR textMsg[256] = L"D2D Demo";
  int msgLen = 0;
  wsprintf(textMsg, L"%i", gs->bullets.size());
  msgLen = wcslen(textMsg);
  frame->renderTarget->DrawTextW(textMsg,msgLen,frame->writeTextFormat.get(),D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),frame->brush.get());

  _ui64tow(pd->fps,textMsg,10);
  msgLen = wcslen(textMsg);
}

void RenderTitleScreen(const std::unique_ptr<d2d_frame>& frame)
{
  D2D1_SIZE_F renderTargetSize = frame->renderTarget->GetSize();

  std::wstring titleText = L"Z - rotate ship left\n";
  titleText += L"X - rotate ship right\n";
  titleText += L"Right mouse button - accelerate\n";
  titleText += L"Left mouse button - shoot\n";
  titleText += L"\nPress SPACE to start";
  frame->renderTarget->DrawTextW(titleText.c_str(),titleText.length(),frame->writeTextFormat.get(),D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),frame->brush.get());
}

void SetTransformAndDrawGameObject(const game_object& gameObject, winrt::com_ptr<ID2D1HwndRenderTarget> renderTarget, winrt::com_ptr<ID2D1SolidColorBrush> brush)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(gameObject.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(gameObject.xPos, gameObject.yPos);
  D2D1_SIZE_F size;
  size.width = RENDER_SCALE_WIDTH;
  size.height = RENDER_SCALE_HEIGHT;
  const D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(size);
  const D2D1::Matrix3x2F transform = rotate * translate * scale;
  renderTarget->SetTransform(transform);
  DrawGameObject(gameObject, renderTarget, brush);
}

void DrawGameObject(const game_object& gameObject, winrt::com_ptr<ID2D1HwndRenderTarget> renderTarget, winrt::com_ptr<ID2D1SolidColorBrush> brush)
{
  D2D1_RECT_F rectangle = D2D1::RectF(- gameObject.size / 2, - gameObject.size / 2, gameObject.size / 2, gameObject.size / 2);
  renderTarget->FillRectangle(&rectangle, brush.get());
}
