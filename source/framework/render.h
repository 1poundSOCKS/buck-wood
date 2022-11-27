#ifndef _render_
#define _render_

#include "framework.h"
#include "dwrite_text_formats.h"

struct render_target
{
  render_target(HWND windowHandle, UINT fps);

  winrt::com_ptr<ID3D11Device> d3dDevice;
  winrt::com_ptr<IDXGISwapChain> dxgiSwapChain;
  winrt::com_ptr<IDXGISurface> dxgiSurface;
  winrt::com_ptr<IDXGIDevice> dxgiDevice;
  winrt::com_ptr<ID2D1Factory> d2dFactory;
  winrt::com_ptr<ID2D1RenderTarget> d2dRenderTarget;
};

struct render_brushes
{
  enum color { color_white, color_green, color_red };

  render_brushes(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget);

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> brushWhite;
  winrt::com_ptr<ID2D1SolidColorBrush> brushGrey;
  winrt::com_ptr<ID2D1SolidColorBrush> brushGreen;
  winrt::com_ptr<ID2D1SolidColorBrush> brushRed;
  winrt::com_ptr<ID2D1SolidColorBrush> brushYellow;
  winrt::com_ptr<ID2D1SolidColorBrush> brushCyan;
};

struct render_text_formats
{
  render_text_formats();

  winrt::com_ptr<IDWriteTextFormat> writeTextFormat;
  winrt::com_ptr<IDWriteTextFormat> menuTextFormat;
  winrt::com_ptr<IDWriteTextFormat> levelTimerTextFormat;
  winrt::com_ptr<IDWriteTextFormat> levelEndTextFormat;
};

struct render_point
{
  render_point(float x, float y, float size, render_brushes::color brushColor = render_brushes::color::color_white);
  
  D2D1_RECT_F rect;
  render_brushes::color brushColor;
};

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, render_brushes::color brushColor = render_brushes::color::color_white);
  
  D2D1_POINT_2F start, end;
  render_brushes::color brushColor;
};

void RenderDiagnostics(const render_brushes& brushes, const render_text_formats& textFormats, const std::vector<std::wstring>& diagnosticsData);
void RenderTimer(const render_brushes& brushes, const render_text_formats& textFormats, float seconds);
void RenderMainScreenPrompt(const render_brushes& brushes, const render_text_formats& textFormats, const std::wstring_view& text);

void RenderMouseCursor(const render_brushes& brushes, float x, float y);
void RenderPoints(const render_brushes& brushes, std::vector<render_point>::const_iterator begin, std::vector<render_point>::const_iterator end);
void RenderPoint(const render_brushes& brushes, const render_point& point);
void RenderLines(const render_brushes& brushes, float renderWidth, std::vector<render_line>::const_iterator begin, std::vector<render_line>::const_iterator end);

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight);
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

#endif
