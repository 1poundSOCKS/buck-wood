#pragma once

class render_brush_def
{
public:

  render_brush_def(D2D_COLOR_F color);
  render_brush_def(D2D1::ColorF color);

  [[nodiscard]] auto CreateBrush() const -> winrt::com_ptr<ID2D1SolidColorBrush>;
  [[nodiscard]] auto CreateBrush(ID2D1RenderTarget* renderTarget) const -> winrt::com_ptr<ID2D1SolidColorBrush>;

private:

  D2D1::ColorF m_color;
};
