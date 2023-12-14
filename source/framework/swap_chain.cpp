#include "pch.h"
#include "swap_chain.h"
#include "directx_functions.h"

auto swap_chain::create(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> void
{
  destroy();
  m_instance = new swap_chain(swapChainDesc, featureLevels);
}

auto swap_chain::device() -> const winrt::com_ptr<ID3D11Device>&
{
  return static_cast<const swap_chain*>(m_instance)->m_d3dDevice;
}

swap_chain::swap_chain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[])
{
  auto [d3dDevice, swapChain] = CreateDeviceAndSwapChain(swapChainDesc, featureLevels);
  m_d3dDevice = d3dDevice;
  m_comObject = swapChain;
}
