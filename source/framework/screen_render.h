#pragma once

winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);
winrt::com_ptr<ID2D1Factory> CreateD2DFactory();
winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);
winrt::com_ptr<IDWriteFactory> CreateDWriteFactory();
winrt::com_ptr<ID2D1PathGeometry> CreatePathGeometry(ID2D1Factory* d2dFactory);
winrt::com_ptr<ID2D1TransformedGeometry> CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1PathGeometry* geometry, const D2D1_MATRIX_3X2_F& transform);
winrt::com_ptr<ID2D1SolidColorBrush> CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color);
winrt::com_ptr<IDWriteTextFormat> CreateScreenRenderTextFormat(IDWriteFactory* writeFactory, LPCWSTR fontFamily, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle,DWRITE_FONT_STRETCH fontStretch, float fontSize);
