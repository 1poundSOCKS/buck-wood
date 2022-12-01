#ifndef _render_
#define _render_

#include "framework.h"

struct render_brushes
{
  enum color { color_white, color_green, color_red };

  render_brushes(ID2D1RenderTarget* renderTarget);

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> brushWhite;
  winrt::com_ptr<ID2D1SolidColorBrush> brushGrey;
  winrt::com_ptr<ID2D1SolidColorBrush> brushGreen;
  winrt::com_ptr<ID2D1SolidColorBrush> brushRed;
  winrt::com_ptr<ID2D1SolidColorBrush> brushYellow;
  winrt::com_ptr<ID2D1SolidColorBrush> brushCyan;
};

struct render_point
{
  render_point(float x, float y, float size, render_brushes::color brushColor = render_brushes::color::color_white);
  render_point(float x, float y, ID2D1SolidColorBrush* brush, float size);
  
  D2D1_RECT_F rect;
  render_brushes::color brushColor;
  ID2D1SolidColorBrush* brush = nullptr;
};

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, render_brushes::color brushColor = render_brushes::color::color_white);
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width = 1);
  
  D2D1_POINT_2F start, end;
  render_brushes::color brushColor;
  ID2D1SolidColorBrush* brush = nullptr;
  float width = 1;
};

void RenderText(
  ID2D1RenderTarget* renderTarget, 
  ID2D1SolidColorBrush* brush, 
  IDWriteTextFormat* textFormat, 
  std::wstring_view text, 
  DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR, 
  DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING);

void RenderPoints(const render_brushes& brushes, std::vector<render_point>::const_iterator begin, std::vector<render_point>::const_iterator end);
void RenderPoint(const render_brushes& brushes, const render_point& point);
void RenderLines(const render_brushes& brushes, float renderWidth, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end);
void RenderLines(ID2D1RenderTarget* renderTarget, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end);

template <typename input_iterator_type>
void RenderPoints(ID2D1RenderTarget* renderTarget, const typename input_iterator_type begin, const typename input_iterator_type end)
{
  for( auto point = begin; point != end; ++point )
  {
    renderTarget->FillRectangle(point->rect, point->brush);
  }
}

const winrt::com_ptr<ID2D1SolidColorBrush>& GetBrush(const render_brushes& brushes, render_brushes::color brushColor);

template <typename T>
void CreateConnectedRenderLines(typename std::vector<T>::const_iterator begin, 
                                typename std::vector<T>::const_iterator end, 
                                std::back_insert_iterator<std::vector<render_line>> insertIterator, 
                                render_brushes::color brushColor=render_brushes::color::color_white, 
                                float x=0, float y=0)
{
  std::transform(std::next(begin), end, begin, insertIterator, [brushColor, x, y](const auto& point2, const auto& point1)
  {
    D2D1_POINT_2F start(point1.x + x, point1.y + y);
    D2D1_POINT_2F end(point2.x + x, point2.y + y);
    return render_line(start, end, brushColor);
  });

  typename std::vector<T>::const_iterator last = std::prev(end);

  D2D1_POINT_2F startPoint(last->x + x, last->y + y);
  D2D1_POINT_2F endPoint(begin->x + x, begin->y + y);
  insertIterator = render_line(startPoint, endPoint, brushColor);
};

template <typename T>
void CreateDisconnectedRenderLines(typename std::vector<T>::const_iterator begin, 
                                   typename std::vector<T>::const_iterator end, 
                                   std::back_insert_iterator<std::vector<render_line>> insertIterator, 
                                   render_brushes::color brushColor=render_brushes::color::color_white, 
                                   float x=0, float y=0)
{
  for( typename std::vector<T>::const_iterator i = begin; i != end; std::advance(i, 2) )
  {
    typename std::vector<T>::const_iterator next = std::next(i);

    if( next != end )
    {
      D2D1_POINT_2F start(i->x + x, i->y + y);
      D2D1_POINT_2F end(next->x + x, next->y + y);
      insertIterator = render_line(start, end, brushColor);
    }
  }
};

template <typename input_iterator_type, typename insert_iterator_type>
void CreateDisconnectedRenderLines(typename input_iterator_type begin, 
                                   typename input_iterator_type end, 
                                   insert_iterator_type insertIterator, 
                                   ID2D1SolidColorBrush* brush, 
                                   float width, float x=0, float y=0)
{
  for( auto i = begin; i != end; std::advance(i, 2) )
  {
    auto next = std::next(i);
    if( next != end ) insertIterator = render_line({i->x + x, i->y + y}, {next->x + x, next->y + y}, brush, width);
  }
};

template <typename input_iterator_type, typename insert_iterator_type>
void CreateConnectedRenderLines(typename input_iterator_type begin, 
                                typename input_iterator_type end, 
                                typename insert_iterator_type insertIterator, 
                                ID2D1SolidColorBrush* brush, 
                                float width, float x=0, float y=0)
{
  std::transform(std::next(begin), end, begin, insertIterator, [brush, width, x, y](const auto& point2, const auto& point1)
  {
    return render_line({point1.x + x, point1.y + y}, {point2.x + x, point2.y + y}, brush, width);
  });

  auto last = std::prev(end);
  insertIterator = render_line({last->x + x, last->y + y}, {begin->x + x, begin->y + y}, brush, width);
};

#endif
