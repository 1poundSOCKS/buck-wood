#pragma once

class render_target_area
{
public:
  render_target_area(ID2D1RenderTarget* renderTarget, float width, float height);
  [[nodiscard]] auto GetRect() const -> D2D1_RECT_F;

private:

  [[nodiscard]] static auto GetRenderTargetRect(ID2D1RenderTarget* renderTarget, float width, float height) -> D2D1_RECT_F;

  D2D1_RECT_F m_rect;
};
