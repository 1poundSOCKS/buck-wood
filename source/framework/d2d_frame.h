#ifndef _d2d_frame_
#define _d2d_frame_

struct d2d_frame
{
  d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget);
  
  d2d_frame(const d2d_frame&) = delete;
  d2d_frame(d2d_frame&& frame) = delete;
  void operator=(const d2d_frame&) = delete;
  
  ~d2d_frame();

  winrt::com_ptr<ID2D1RenderTarget> renderTarget;
};

#endif
