#include "render.h"
#include <string>

void RenderDiagnostics(const d2d_frame& frame, const diagnostics_data& diagnosticsData)
{
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);

  rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  std::wstring msg;
  for( const auto& text: diagnosticsData )
  {
    msg += text;
    msg += L"\n";
  }

  frame.renderTarget->DrawTextW(msg.c_str(),msg.length(), frame.textFormats.writeTextFormat.get(), rect, frame.brushes.brushDiagnostics.get());
}

void RenderTimer(const d2d_frame& frame, float seconds)
{
  D2D_SIZE_F size = frame.renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(size.width * 7 / 8, size.height / 16, size.width - 1, size.height * 3 / 16);

  frame.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  static wchar_t timeText[64];
  swprintf(timeText, L"%.2f", seconds);
  frame.renderTarget->DrawTextW(timeText,wcslen(timeText), frame.textFormats.levelTimerTextFormat.get(), rect, frame.brushes.brushTimer.get());
}

void RenderMouseCursor(const d2d_frame& frame, const mouse_cursor& mouseCursor)
{
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(frame.renderTargetMouseX, frame.renderTargetMouseY);
  frame.renderTarget->SetTransform(translate);
  RenderLines(mouseCursor.lines, frame.renderTarget, frame.brushes.brush);
}

void RenderPlayer(const d2d_frame& frame, const player_ship& player)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(player.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(player.xPos, player.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * frame.viewTransform;
  frame.renderTarget->SetTransform(transform);
  RenderShape(*player.outline, frame.renderTarget, frame.brushes.brush);

  if( player.thrusterOn )
  {
    D2D1_POINT_2F startPoint;
    startPoint.x = player.thruster->start.x;
    startPoint.y = player.thruster->start.y;

    D2D1_POINT_2F endPoint;
    endPoint.x = player.thruster->end.x;
    endPoint.y = player.thruster->end.y;

    frame.renderTarget->DrawLine(startPoint, endPoint, frame.brushes.brushThrusters.get(), 6.0f);
  }
}

void RenderBullet(const d2d_frame& frame, const bullet& bullet)
{
  static const float bulletSize = 3.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  const D2D1::Matrix3x2F transform = translate * frame.viewTransform;
  frame.renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  frame.renderTarget->FillRectangle(&rectangle, frame.brushes.brush.get());
}

void RenderLevel(const d2d_frame& frame, const game_level& level)
{
  frame.renderTarget->SetTransform(frame.viewTransform);
  RenderShape(*level.boundary, frame.renderTarget, frame.brushes.brush);

  for( const auto& shape: level.objects )
  {
    RenderShape(*shape, frame.renderTarget, frame.brushes.brush);
  }

  for( const auto& target: level.targets)
  {
    const winrt::com_ptr<ID2D1SolidColorBrush>& targetBrush = target->state == 
      target::ACTIVATED ? frame.brushes.brushActivated : frame.brushes.brushDeactivated;
    
    RenderShape(target->shape, frame.renderTarget, targetBrush);
  }
}

void RenderHighlightedPoint(const d2d_frame& frame, const game_point& point)
{
  static const float pointSize = 5.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(point.x, point.y);
  const D2D1::Matrix3x2F transform = translate * frame.viewTransform;
  frame.renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- pointSize / 2, - pointSize / 2, pointSize / 2, pointSize / 2);
  frame.renderTarget->FillRectangle(&rectangle, frame.brushes.brushActivated.get());
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

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight)
{
  return D2D1::Matrix3x2F::Translation(-centerPosX, -centerPosY) * 
    D2D1::Matrix3x2F::Scale(scale, scale) *
    D2D1::Matrix3x2F::Translation(renderTargetWidth / 2, renderTargetHeight / 2);
}
