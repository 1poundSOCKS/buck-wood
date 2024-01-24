#pragma once

#define PI 3.14159265f
#define DEGTORAD(D)((D * PI) / 180.0f)
#define RADTODEG(R)((180.0f * R) / PI)

using d3d_device_and_swap_chain = std::tuple<winrt::com_ptr<ID3D11Device>, winrt::com_ptr<IDXGISwapChain>>;

[[nodiscard]] auto CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> d3d_device_and_swap_chain;
[[nodiscard]] auto GetDXGIDevice(ID3D11Device* d3dDevice) -> winrt::com_ptr<IDXGIDevice>;
[[nodiscard]] auto CreateDWriteFactory() -> winrt::com_ptr<IDWriteFactory>;
[[nodiscard]] auto CreateScreenRenderTextFormat(IDWriteFactory* writeFactory, LPCWSTR fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle,DWRITE_FONT_STRETCH fontStretch, float fontSize) -> winrt::com_ptr<IDWriteTextFormat>;
[[nodiscard]] auto CreateDirectInput(HINSTANCE instance) -> winrt::com_ptr<IDirectInput8>;
[[nodiscard]] auto CreateKeyboard(IDirectInput8* directInput, HWND window) -> winrt::com_ptr<IDirectInputDevice8>;
