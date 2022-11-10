#include "render.h"
#include <string>

render_point::render_point(float x, float y, float size)
{
  float halfSize = size / 2;
  rect.left = x - halfSize;
  rect.right = x + halfSize;
  rect.top = y - halfSize;
  rect.bottom = y + halfSize;
}

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end)
: start(start), end(end)
{
}

void RenderDiagnostics(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const diagnostics_data& diagnosticsData, const dwrite_text_formats& textFormats, const d2d_brushes& brushes)
{
  D2D_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);

  rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  std::wstring msg;
  for( const auto& text: diagnosticsData )
  {
    msg += text;
    msg += L"\n";
  }

  renderTarget->DrawTextW(msg.c_str(),msg.length(), textFormats.writeTextFormat.get(), rect, brushes.brushDiagnostics.get());
}

void RenderTimer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float seconds, const dwrite_text_formats& textFormats, const d2d_brushes& brushes)
{
  D2D_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(size.width * 7 / 8, size.height / 16, size.width - 1, size.height * 3 / 16);

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  static wchar_t timeText[64];
  swprintf(timeText, L"%.2f", seconds);
  renderTarget->DrawTextW(timeText,wcslen(timeText), textFormats.levelTimerTextFormat.get(), rect, brushes.brushTimer.get());
}

void RenderMainScreenPrompt(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
                            const winrt::com_ptr<IDWriteTextFormat>& textFormat, 
                            const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
                            const std::wstring& text)
{
  D2D_SIZE_F size = renderTarget->GetSize();
  D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  renderTarget->DrawTextW(text.c_str(),text.length(), textFormat.get(), rect, brush.get());
}

void RenderMouseCursor(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const mouse_cursor& mouseCursor, float x, float y, const d2d_brushes& brushes)
{
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(x, y);
  renderTarget->SetTransform(translate);
  RenderLines(mouseCursor.lines, renderTarget, brushes.brushTimer);
}

void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const player_ship& player, const d2d_brushes& brushes)
{
  RenderPlayer(renderTarget, viewTransform, player, brushes.brush, brushes.brushThrusters);
}

void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, 
                  const D2D1::Matrix3x2F& viewTransform, const player_ship& player, 
                  const winrt::com_ptr<ID2D1SolidColorBrush>& brush, 
                  const winrt::com_ptr<ID2D1SolidColorBrush>& brushThrusters)
{
  const D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(player.angle,D2D1::Point2F(0,0));
  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(player.xPos, player.yPos);
  const D2D1::Matrix3x2F transform = rotate * translate * viewTransform;
  renderTarget->SetTransform(transform);
  RenderShape(*player.outline, renderTarget, brush);

  if( player.thrusterOn )
  {
    D2D1_POINT_2F startPoint;
    startPoint.x = player.thruster->start.x;
    startPoint.y = player.thruster->start.y;

    D2D1_POINT_2F endPoint;
    endPoint.x = player.thruster->end.x;
    endPoint.y = player.thruster->end.y;

    renderTarget->DrawLine(startPoint, endPoint, brushThrusters.get(), 6.0f);
  }
}

void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const d2d_brushes& brushes)
{
  static const float bulletSize = 3.0f;

  const D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  const D2D1::Matrix3x2F transform = translate * viewTransform;
  renderTarget->SetTransform(transform);
  D2D1_RECT_F rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  renderTarget->FillRectangle(&rectangle, brushes.brush.get());
}

void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level& level, const d2d_brushes& brushes)
{
  renderTarget->SetTransform(viewTransform);
  RenderShape(*level.boundary, renderTarget, brushes.brush);

  for( const auto& shape: level.objects )
  {
    RenderShape(*shape, renderTarget, brushes.brush);
  }

  for( const auto& target: level.targets)
  {
    const winrt::com_ptr<ID2D1SolidColorBrush>& targetBrush = target->state == 
      target::ACTIVATED ? brushes.brushActivated : brushes.brushDeactivated;
    
    RenderShape(target->shape, renderTarget, targetBrush);
  }
}

void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level_edit& level, const d2d_brushes& brushes)
{
  renderTarget->SetTransform(viewTransform);
  RenderShape(*level.boundary, renderTarget, brushes.brush);

  for( const auto& shape: level.objects )
  {
    RenderShape(*shape, renderTarget, brushes.brush);
  }

  for( const auto& target: level.targets)
  {
    game_shape targetShape;
    InitializeTargetShape(target.x, target.y, target.size, targetShape);
    RenderShape(targetShape, renderTarget, brushes.brushDeactivated);
  }
}

void RenderPoint(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_point& point, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  renderTarget->FillRectangle(&point.rect, brush.get());
}

void RenderLine(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_line& line, float renderWidth, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  renderTarget->DrawLine(line.start, line.end, brush.get(), renderWidth);
}

void RenderTarget(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const target_edit& target, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  game_shape targetShape;
  InitializeTargetShape(target.x, target.y, target.size, targetShape);
  renderTarget->SetTransform(viewTransform);
  RenderShape(targetShape, renderTarget, brush);
}

void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  RenderLines(shape.lines, renderTarget, brush);
}

void RenderShape(game_shape_edit& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
{
  std::list<game_line_edit> lines;
  CreateShapeLinesFromPoints(lines, shape.points);
  RenderLines(lines, renderTarget, brush);
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

void RenderLines(const std::list<game_line_edit>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush)
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
