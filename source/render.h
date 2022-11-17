#ifndef _render_
#define _render_

#define UNICODE

#include <memory>
#include <tchar.h>

#include "framework/framework.h"
#include "game_objects.h"
#include "dwrite_text_formats.h"
#include "d2d_brushes.h"


struct render_brushes
{
  enum color { color_white, color_green, color_red };

  render_brushes(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget);

  winrt::com_ptr<ID2D1SolidColorBrush> brushWhite;
  winrt::com_ptr<ID2D1SolidColorBrush> brushGreen;
  winrt::com_ptr<ID2D1SolidColorBrush> brushRed;
};

struct render_point
{
  render_point(float x, float y, float size, render_brushes::color brushColor);
  
  D2D1_RECT_F rect;
  render_brushes::color brushColor;
};

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, render_brushes::color brushColor = render_brushes::color::color_white);
  
  D2D1_POINT_2F start, end;
  render_brushes::color brushColor;
};

void RenderDiagnostics(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const std::vector<std::wstring>& diagnosticsData, const dwrite_text_formats& textFormats, const d2d_brushes& brushes);
void RenderTimer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float seconds, const dwrite_text_formats& textFormats, const d2d_brushes& brushes);
void RenderMainScreenPrompt(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<IDWriteTextFormat>& textFormat, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const std::wstring& text);
void RenderMouseCursor(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const mouse_cursor& mouseCursor, float x, float y, const d2d_brushes& brushes);
void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const player_ship& player, const d2d_brushes& brushes);
void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const player_ship& player, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<ID2D1SolidColorBrush>& brushThrusters);
void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const d2d_brushes& brushes);
void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level& level, const d2d_brushes& brushes);
void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);

void RenderMouseCursor(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float x, float y, const render_brushes& brushes);
void RenderLines(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end);
void RenderPoints(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_brushes& brushes, std::vector<render_point>::const_iterator begin, std::vector<render_point>::const_iterator end);
void RenderPoint(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_point& point, const render_brushes& brushes);
void RenderLines(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_brushes& brushes, float renderWidth, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end);
void RenderLine(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_line& line, float renderWidth, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight);
const winrt::com_ptr<ID2D1SolidColorBrush>& GetBrush(const render_brushes& brushes, render_brushes::color brushColor);

template <typename T>
void CreateConnectedRenderLines(typename std::vector<T>::const_iterator begin, typename std::vector<T>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator, float x, float y)
{
  std::transform(std::next(begin), end, begin, insertIterator, [x,y](const auto& point2, const auto& point1)
  {
    D2D1_POINT_2F start(point1.x + x, point1.y + y);
    D2D1_POINT_2F end(point2.x + x, point2.y + y);
    return render_line(start, end);
  });

  typename std::vector<T>::const_iterator last = std::prev(end);

  D2D1_POINT_2F startPoint(last->x + x, last->y + y);
  D2D1_POINT_2F endPoint(begin->x + x, begin->y + y);
  insertIterator = render_line(startPoint, endPoint);
};

template <typename T>
void CreateDisconnectedRenderLines(typename std::vector<T>::const_iterator begin, typename std::vector<T>::const_iterator end, std::back_insert_iterator<std::vector<render_line>> insertIterator, float x, float y)
{
  for( typename std::vector<T>::const_iterator i = begin; i != end; std::advance(i, 2) )
  {
    typename std::vector<T>::const_iterator next = std::next(i);

    if( next != end )
    {
      D2D1_POINT_2F start(i->x + x, i->y + y);
      D2D1_POINT_2F end(next->x + x, next->y + y);
      insertIterator = render_line(start, end);
    }
  }
};

#endif
