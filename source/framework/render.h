#ifndef _render_
#define _render_

struct render_point
{
  D2D1_RECT_F rect;
  ID2D1SolidColorBrush* brush;
};

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width = 1);
  
  D2D1_POINT_2F start, end;
  ID2D1SolidColorBrush* brush = nullptr;
  float width = 1;
};

using render_point_inserter_type = std::back_insert_iterator<std::vector<render_point>>;
using render_line_inserter_type = std::back_insert_iterator<std::vector<render_line>>;
using render_rect = render_point;

void RenderText(
  ID2D1RenderTarget* renderTarget, 
  ID2D1SolidColorBrush* brush, 
  IDWriteTextFormat* textFormat, 
  std::wstring_view text, 
  DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR, 
  DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING);

template <typename input_iterator_type>
void RenderPoints(
  ID2D1RenderTarget* renderTarget, 
  const typename input_iterator_type begin, 
  const typename input_iterator_type end)
{
  for( auto point = begin; point != end; ++point )
  {
    renderTarget->FillRectangle( point->rect, point->brush);
  }
}

template <typename input_iterator_type>
void RenderLines(
  ID2D1RenderTarget* renderTarget, 
  input_iterator_type begin, 
  input_iterator_type end)
{
  for( auto line = begin; line != end; ++line )
  {
    renderTarget->DrawLine(line->start, line->end, line->brush, line->width);
  }
}

template <typename input_iterator_type>
void RenderRectangles(
  ID2D1RenderTarget* renderTarget, 
  const typename input_iterator_type begin, 
  const typename input_iterator_type end)
{
  for( auto rect = begin; rect != end; ++rect )
  {
    renderTarget->FillRectangle( rect->rect, rect->brush);
  }
}

template <typename input_iterator_type, typename insert_iterator_type>
void CreateDisconnectedRenderLines(
  typename input_iterator_type begin, 
  typename input_iterator_type end, 
  insert_iterator_type insertIterator, 
  ID2D1SolidColorBrush* brush, 
  float width, 
  float x=0, 
  float y=0)
{
  for( auto i = begin; i != end; std::advance(i, 2) )
  {
    auto next = std::next(i);
    if( next != end ) insertIterator = render_line({i->x + x, i->y + y}, {next->x + x, next->y + y}, brush, width);
  }
};

template <typename input_iterator_type, typename insert_iterator_type>
void CreateConnectedRenderLines(
  typename input_iterator_type begin, 
  typename input_iterator_type end, 
  typename insert_iterator_type insertIterator, 
  ID2D1SolidColorBrush* brush, 
  float width, 
  float x=0, 
  float y=0,
  bool loop=true)
{
  std::transform(std::next(begin), end, begin, insertIterator, [brush, width, x, y](const auto& point2, const auto& point1)
  {
    return render_line({point1.x + x, point1.y + y}, {point2.x + x, point2.y + y}, brush, width);
  });

  if( loop )
  {
    auto last = std::prev(end);
    insertIterator = render_line({last->x + x, last->y + y}, {begin->x + x, begin->y + y}, brush, width);
  }
};

template <typename input_iterator_type, typename insert_iterator_type>
void CreateRenderLines(
  input_iterator_type begin, 
  input_iterator_type end, 
  insert_iterator_type insertIterator,
  ID2D1SolidColorBrush* brush, 
  float width)
{
  std::transform(
    begin, 
    end, 
    insertIterator,
    [brush, width](auto& line)
    {
      return render_line
      {
        D2D1_POINT_2F { line.start.x, line.start.y },
        D2D1_POINT_2F { line.end.x, line.end.y },
        brush,
        width
      };
    }
  );
}

#endif
