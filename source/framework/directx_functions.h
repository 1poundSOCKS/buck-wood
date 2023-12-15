#pragma once

using d3d_device_and_swap_chain = std::tuple<winrt::com_ptr<ID3D11Device>, winrt::com_ptr<IDXGISwapChain>>;

auto CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> d3d_device_and_swap_chain;
auto GetDXGIDevice(ID3D11Device* d3dDevice) -> winrt::com_ptr<IDXGIDevice>;
auto CreateD2DFactory() -> winrt::com_ptr<ID2D1Factory>;
auto CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1RenderTarget>;
auto CreateDWriteFactory() -> winrt::com_ptr<IDWriteFactory>;
auto CreatePathGeometry(ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1PathGeometry>;
auto CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1Geometry* geometry, const D2D1_MATRIX_3X2_F& transform) -> winrt::com_ptr<ID2D1TransformedGeometry>;
auto CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color) -> winrt::com_ptr<ID2D1SolidColorBrush>;
auto CreateScreenRenderTextFormat(IDWriteFactory* writeFactory, LPCWSTR fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle,DWRITE_FONT_STRETCH fontStretch, float fontSize) -> winrt::com_ptr<IDWriteTextFormat>;
auto CreateDirectSound(HWND window) -> winrt::com_ptr<IDirectSound8>;
auto CreatePrimarySoundBuffer(IDirectSound8* directSound) -> winrt::com_ptr<IDirectSoundBuffer>;
auto CreateDirectInput(HINSTANCE instance) -> winrt::com_ptr<IDirectInput8>;
auto CreateKeyboard(IDirectInput8* directInput, HWND window) -> winrt::com_ptr<IDirectInputDevice8>;
