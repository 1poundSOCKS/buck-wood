#pragma once

#include "com_singleton.h"

class swap_chain : public com_singleton<IDXGISwapChain>
{

public:

  static auto create(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator) -> void;

private:

  swap_chain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);

};
