#include "pch.h"
#include "swap_chain.h"
#include "directx_functions.h"

auto swap_chain::create(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator) -> void
{
  destroy();
  m_instance = new swap_chain(window, refreshRateNumerator, refreshRateDenominator);
}

swap_chain::swap_chain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator) : 
  com_singleton(CreateSwapChain(window, refreshRateNumerator, refreshRateDenominator))
{
}
