#include "pch.h"
#include "render_target.h"
#include "dwrite_factory.h"

render_target* render_target::m_instance = nullptr;

auto render_target::create(HWND wnd, ID2D1Factory* d2dFactory) -> void
{
  m_instance = new render_target(wnd, d2dFactory);
}

auto render_target::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

render_target::render_target(HWND wnd, ID2D1Factory* d2dFactory)
{
  m_swapChain = CreateSwapChain(wnd, render_target::fps(), 1);
  m_renderTarget = CreateRenderTarget(m_swapChain.get(), d2dFactory);
  m_swapChain->SetFullscreenState(FALSE, NULL);
}

auto render_target::RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_renderTarget->DrawText(text.data(), static_cast<UINT32>(text.length()), textFormat, rect, brush);
}

auto render_target::RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  D2D1_SIZE_F rect = m_renderTarget->GetSize();
  RenderText(D2D1_RECT_F { 0, 0, rect.width - 1, rect.height - 1 }, brush, textFormat, text);
}

winrt::com_ptr<IDXGISwapChain> render_target::CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator)
{
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Width = 1920;
  swapChainDesc.BufferDesc.Height = 1080;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRateNumerator;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRateDenominator;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = window;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = TRUE;

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };

  winrt::com_ptr<ID3D11Device> d3dDevice;
  winrt::com_ptr<IDXGISwapChain> swapChain;

  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
    featureLevels, 3, D3D11_SDK_VERSION, &swapChainDesc, swapChain.put(), d3dDevice.put(), NULL, NULL);

  if( FAILED(hr) ) throw L"error";

  winrt::com_ptr<IDXGIDevice> dxgiDevice; // NOT USED: gets thrown away
  hr = d3dDevice->QueryInterface(dxgiDevice.put());
  if( FAILED(hr) ) throw L"error";

  return swapChain;
}

winrt::com_ptr<ID2D1RenderTarget> render_target::CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory)
{
  winrt::com_ptr<ID2D1RenderTarget> renderTarget;

  winrt::com_ptr<IDXGISurface> dxgi_surface;
  HRESULT hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgi_surface.put_void());
  if( FAILED(hr) ) throw L"error";

  D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
  );
  
  hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgi_surface.get(), props, renderTarget.put());
  if( FAILED(hr) ) throw L"error";

  return renderTarget;
}
