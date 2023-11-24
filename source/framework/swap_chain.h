#pragma once

class swap_chain
{

public:

  static auto create(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator) -> void;
  static auto destroy() -> void;
  static auto get() -> const winrt::com_ptr<IDXGISwapChain>&;
  static auto get_raw() -> IDXGISwapChain*;

  static auto present() -> void;
  static auto unlockFrameRate() -> void;
  static auto fullScreen() -> void;

  [[nodiscard]] static auto isFrameRateUnlocked() -> bool;

private:

  swap_chain(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator);

private:

  inline static swap_chain* m_instance { nullptr };
  winrt::com_ptr<IDXGISwapChain> m_swapChain;
  bool m_unlockFrameRate { false };

};

inline auto swap_chain::create(HWND window, UINT refreshRateNumerator, UINT refreshRateDenominator) -> void
{
  destroy();
  m_instance = new swap_chain(window, refreshRateNumerator, refreshRateDenominator);
}

inline auto swap_chain::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

inline auto swap_chain::get() -> const winrt::com_ptr<IDXGISwapChain>&
{
  return m_instance->m_swapChain;
}

inline auto swap_chain::get_raw() -> IDXGISwapChain*
{
  return m_instance->m_swapChain.get();
}

inline auto swap_chain::present() -> void
{
  m_instance->m_swapChain->Present(m_instance->isFrameRateUnlocked() ? 0 : 1, 0);
}

[[nodiscard]] inline auto swap_chain::isFrameRateUnlocked() -> bool
{
  return m_instance->m_unlockFrameRate;
}

inline auto swap_chain::unlockFrameRate() -> void
{
  m_instance->m_unlockFrameRate = true;
}

inline auto swap_chain::fullScreen() -> void
{
  m_instance->m_swapChain->SetFullscreenState(TRUE, nullptr);
}
