#pragma once

class render_target
{
public:

  // static auto create(HWND wnd, ID2D1Factory* d2dFactory) -> void;
  static auto create(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> void;
  static auto destroy() -> void;

  // [[nodiscard]] static auto swapChain() -> winrt::com_ptr<IDXGISwapChain>&;
  [[nodiscard]] static auto renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&;
  [[nodiscard]] static auto fps() -> int;
  // [[nodiscard]] static auto isFrameRateUnlocked() -> bool;

  // static auto present() -> void;
  // static auto unlockFrameRate() -> void;
  // static auto fullScreen() -> void;

  static auto renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  static auto renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

private:

  render_target(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);

  auto RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  auto RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

  // static winrt::com_ptr<IDXGISwapChain> CreateSwapChain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);
  // static winrt::com_ptr<ID2D1RenderTarget> CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);

private:

  static render_target* m_instance;
  HINSTANCE m_appInstance = nullptr;
  // winrt::com_ptr<IDXGISwapChain> m_swapChain;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  // bool m_unlockFrameRate { false };

};

// [[nodiscard]] inline auto render_target::swapChain() -> winrt::com_ptr<IDXGISwapChain>&
// {
//   return m_instance->m_swapChain;
// }

[[nodiscard]] inline auto render_target::renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&
{
  return m_instance->m_renderTarget;
}

// inline auto render_target::present() -> void
// {
//   m_instance->m_swapChain->Present(m_instance->isFrameRateUnlocked() ? 0 : 1, 0);
// }

[[nodiscard]] inline auto render_target::fps() -> int
{
  return 60;
}

// [[nodiscard]] inline auto render_target::isFrameRateUnlocked() -> bool
// {
//   return m_instance->m_unlockFrameRate;
// }

// inline auto render_target::unlockFrameRate() -> void
// {
//   m_instance->m_unlockFrameRate = true;
// }

// inline auto render_target::fullScreen() -> void
// {
//   m_instance->m_swapChain->SetFullscreenState(TRUE, nullptr);
// }

inline auto render_target::renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(brush, textFormat, text);
}

inline auto render_target::renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(rect, brush, textFormat, text);
}
