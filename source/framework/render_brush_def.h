#pragma once

class render_brush_def
{
public:

  render_brush_def(D2D1::ColorF color);
  auto CreateBrush(ID2D1RenderTarget* renderTarget) const -> winrt::com_ptr<ID2D1SolidColorBrush>;

private:

  D2D1::ColorF m_color;
};
