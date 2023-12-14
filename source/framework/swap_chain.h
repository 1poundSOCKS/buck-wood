#pragma once

#include "com_singleton.h"

class swap_chain : public com_singleton<IDXGISwapChain>
{

public:

  static auto create(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]) -> void;
  static auto device() -> const winrt::com_ptr<ID3D11Device>&;

private:

  swap_chain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, D3D_FEATURE_LEVEL featureLevels[]);

private:

  winrt::com_ptr<ID3D11Device> m_d3dDevice;

};
