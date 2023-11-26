#pragma once

#include "com_singleton.h"

class render_target : public com_singleton<ID2D1RenderTarget>
{
public:

  static auto create(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> void;

private:

  render_target(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory);

};
