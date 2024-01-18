#include "pch.h"
#include "direct2d_functions.h"
#include "com_logger.h"

auto direct2d::CreateD2DFactory() -> winrt::com_ptr<ID2D1Factory>
{
  winrt::com_ptr<ID2D1Factory> d2dFactory;	
  HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,d2dFactory.put());
  com_logger::fatal(log::type::info, hr, "CreateD2DFactory");
  return d2dFactory;
}

auto direct2d::CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1RenderTarget>
{
  winrt::com_ptr<ID2D1RenderTarget> renderTarget;

  winrt::com_ptr<IDXGISurface> dxgi_surface;
  HRESULT hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgi_surface.put_void());
  com_logger::fatal(log::type::info, hr, "[IDXGISwapChain] GetBuffer");

  D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE));
  hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgi_surface.get(), props, renderTarget.put());
  com_logger::fatal(log::type::info, hr, "[ID2D1Factory] CreateDxgiSurfaceRenderTarget");

  return renderTarget;
}

auto direct2d::CreatePathGeometry(ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1PathGeometry>
{
  winrt::com_ptr<ID2D1PathGeometry> pathGeometry;
  HRESULT hr = d2dFactory->CreatePathGeometry(pathGeometry.put());
  com_logger::fatal(log::type::debug, hr, "[ID2D1Factory] CreatePathGeometry");
  return pathGeometry;
}

auto direct2d::CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1Geometry* geometry, const D2D1_MATRIX_3X2_F& transform) -> winrt::com_ptr<ID2D1TransformedGeometry>
{
  winrt::com_ptr<ID2D1TransformedGeometry> transformedGeometry;
  HRESULT hr = d2dFactory->CreateTransformedGeometry(geometry, transform, transformedGeometry.put());
  com_logger::fatal(log::type::debug, hr, "[ID2D1Factory] CreateTransformedGeometry");
  return transformedGeometry;
}

auto direct2d::CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color) -> winrt::com_ptr<ID2D1SolidColorBrush>
{
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  HRESULT hr = renderTarget->CreateSolidColorBrush(color, brush.put());
  com_logger::fatal(log::type::debug, hr, "[ID2D1RenderTarget] CreateSolidColorBrush");
  return brush;
}
