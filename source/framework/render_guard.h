#ifndef _render_guard_
#define _render_guard_

struct render_guard
{
  render_guard(winrt::com_ptr<ID2D1RenderTarget> renderTarget);
  
  render_guard(const render_guard&) = delete;
  render_guard(render_guard&& frame) = delete;
  void operator=(const render_guard&) = delete;
  
  ~render_guard();

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
};

#endif
