#include "pch.h"
#include "swap_chain.h"
#include "directx_functions.h"

swap_chain::swap_chain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator) : m_swapChain { CreateSwapChain(window, refreshRateNumerator, refreshRateDenominator) }
{
}
