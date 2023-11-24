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
