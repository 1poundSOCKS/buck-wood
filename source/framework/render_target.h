#pragma once

class render_target
{
public:

  static auto create(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&;
  [[nodiscard]] static auto fps() -> int;

  static auto renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  static auto renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

private:

  render_target(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);

  auto RenderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;
  auto RenderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void;

private:

  static render_target* m_instance;
  HINSTANCE m_appInstance = nullptr;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;

};

[[nodiscard]] inline auto render_target::renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&
{
  return m_instance->m_renderTarget;
}

[[nodiscard]] inline auto render_target::fps() -> int
{
  return 60;
}

inline auto render_target::renderText(ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(brush, textFormat, text);
}

inline auto render_target::renderText(const D2D1_RECT_F& rect, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const std::wstring_view& text) -> void
{
  m_instance->RenderText(rect, brush, textFormat, text);
}
