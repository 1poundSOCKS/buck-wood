#pragma once

class render_target
{
public:

  static auto create(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto get() -> const winrt::com_ptr<ID2D1RenderTarget>&;
  [[nodiscard]] static auto get_raw() -> ID2D1RenderTarget*;

private:

  render_target(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);

private:

  static render_target* m_instance;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;

};

[[nodiscard]] inline auto render_target::get() -> const winrt::com_ptr<ID2D1RenderTarget>&
{
  return m_instance->m_renderTarget;
}

[[nodiscard]] inline auto render_target::get_raw() -> ID2D1RenderTarget*
{
  return m_instance->m_renderTarget.get();
}
