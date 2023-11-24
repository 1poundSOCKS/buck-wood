#include "pch.h"
#include "screen_render.h"

winrt::com_ptr<ID2D1Factory> CreateD2DFactory()
{
  winrt::com_ptr<ID2D1Factory> d2dFactory;
	
  HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,d2dFactory.put());
  if( FAILED(hr) ) throw L"error";
  
  return d2dFactory;
}

winrt::com_ptr<IDWriteFactory> CreateDWriteFactory()
{
  winrt::com_ptr<IDWriteFactory> dwriteFactory;
  HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(dwriteFactory),reinterpret_cast<IUnknown**>(dwriteFactory.put()));
  if( FAILED(hr) ) throw L"error";
  return dwriteFactory;
}

winrt::com_ptr<ID2D1PathGeometry> CreatePathGeometry(ID2D1Factory* d2dFactory)
{
  winrt::com_ptr<ID2D1PathGeometry> pathGeometry;

  HRESULT hr = d2dFactory->CreatePathGeometry(pathGeometry.put());
  if( FAILED(hr) ) throw L"error";

  return pathGeometry;
}

winrt::com_ptr<ID2D1TransformedGeometry> CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform)
{
  winrt::com_ptr<ID2D1TransformedGeometry> transformedGeometry;
  HRESULT hr = d2dFactory->CreateTransformedGeometry(geometry, transform, transformedGeometry.put());
  if( FAILED(hr) ) throw std::exception();
  return transformedGeometry;
}

winrt::com_ptr<ID2D1SolidColorBrush> CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color)
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  HRESULT hr = renderTarget->CreateSolidColorBrush(color, brush.put());
  if( FAILED(hr) ) throw(L"error");
  return brush;
}

winrt::com_ptr<IDWriteTextFormat> CreateScreenRenderTextFormat(
  IDWriteFactory* writeFactory, 
  LPCWSTR fontFamily, 
  DWRITE_FONT_WEIGHT fontWeight, 
  DWRITE_FONT_STYLE fontStyle,
  DWRITE_FONT_STRETCH fontStretch,
  float fontSize)
{
  winrt::com_ptr<IDWriteTextFormat> textFormat;
  HRESULT hr = writeFactory->CreateTextFormat(fontFamily, NULL, fontWeight, fontStyle, fontStretch, fontSize, L"", textFormat.put());
  if( FAILED(hr) ) throw L"error";
  return textFormat;
}
