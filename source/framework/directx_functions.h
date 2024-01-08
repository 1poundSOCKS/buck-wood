#pragma once

using d3d_device_and_swap_chain = std::tuple<winrt::com_ptr<ID3D11Device>, winrt::com_ptr<IDXGISwapChain>>;

[[nodiscard]] auto CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> d3d_device_and_swap_chain;
[[nodiscard]] auto GetDXGIDevice(ID3D11Device* d3dDevice) -> winrt::com_ptr<IDXGIDevice>;
[[nodiscard]] auto CreateD2DFactory() -> winrt::com_ptr<ID2D1Factory>;
[[nodiscard]] auto CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1RenderTarget>;
[[nodiscard]] auto CreateDWriteFactory() -> winrt::com_ptr<IDWriteFactory>;
[[nodiscard]] auto CreatePathGeometry(ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1PathGeometry>;
[[nodiscard]] auto CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1Geometry* geometry, const D2D1_MATRIX_3X2_F& transform) -> winrt::com_ptr<ID2D1TransformedGeometry>;
[[nodiscard]] auto CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color) -> winrt::com_ptr<ID2D1SolidColorBrush>;
[[nodiscard]] auto CreateScreenRenderTextFormat(IDWriteFactory* writeFactory, LPCWSTR fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle,DWRITE_FONT_STRETCH fontStretch, float fontSize) -> winrt::com_ptr<IDWriteTextFormat>;
[[nodiscard]] auto CreateDirectSound(HWND window) -> winrt::com_ptr<IDirectSound8>;
[[nodiscard]] auto CreatePrimarySoundBuffer(IDirectSound8* directSound) -> winrt::com_ptr<IDirectSoundBuffer>;
[[nodiscard]] auto CreateDirectInput(HINSTANCE instance) -> winrt::com_ptr<IDirectInput8>;
[[nodiscard]] auto CreateKeyboard(IDirectInput8* directInput, HWND window) -> winrt::com_ptr<IDirectInputDevice8>;

auto LoadPathGeometry(ID2D1PathGeometry* geometry, std::ranges::input_range auto&& points) -> void
{
  winrt::com_ptr<ID2D1GeometrySink> sink;
  geometry->Open(sink.put());
  auto begin = std::ranges::begin(points);

  sink->BeginFigure({ (*begin).x, (*begin).y }, D2D1_FIGURE_BEGIN_FILLED);

  for( auto next = std::ranges::next(begin); next != std::ranges::end(points); ++next )
  {
    sink->AddLine({ (*next).x, (*next).y });
  }

  sink->EndFigure(D2D1_FIGURE_END_CLOSED);
  sink->Close();
}

[[nodiscard]] auto CreatePathGeometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points) -> winrt::com_ptr<ID2D1PathGeometry>
{
  winrt::com_ptr<ID2D1PathGeometry> geometry { CreatePathGeometry(d2dFactory) };
  LoadPathGeometry(geometry.get(), points);
  return geometry;
}

inline [[nodiscard]] auto GetSourceGeometry(ID2D1TransformedGeometry* geometry) -> winrt::com_ptr<ID2D1Geometry>
{
  winrt::com_ptr<ID2D1Geometry> sourceGeometry;
  geometry->GetSourceGeometry(sourceGeometry.put());
  return sourceGeometry;
}

namespace direct2d
{
  inline auto GetDistanceBetweenPoints(D2D1_POINT_2F point1, D2D1_POINT_2F point2) -> float
  {
    float cx = point2.x - point1.x;
    float cy = point2.y - point1.y;
    return sqrt( cx * cx + cy * cy );
  }

  inline auto GetRadiusFromBounds(D2D1_RECT_F bounds) -> float
  {
    float topLeft = GetDistanceBetweenPoints({0, 0}, {bounds.left, bounds.top});
    float topRight = GetDistanceBetweenPoints({0, 0}, {bounds.right, bounds.top});
    float bottomLeft = GetDistanceBetweenPoints({0, 0}, {bounds.left, bounds.bottom});
    float bottomRight = GetDistanceBetweenPoints({0, 0}, {bounds.right, bounds.bottom});
    return std::max({topLeft, topRight, bottomLeft, bottomRight});
  }

  inline auto GetGeometryRadius(ID2D1Geometry* geometry) -> float
  {
    D2D1_RECT_F bounds;
    return SUCCEEDED(geometry->GetBounds(NULL, &bounds)) ? GetRadiusFromBounds(bounds) : 0;
  }
}
