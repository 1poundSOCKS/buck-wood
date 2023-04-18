#include "pch.h"
#include "framework.h"

framework* framework::m_framework = nullptr;
std::mt19937 g_rng; // pseudo-random generator

auto framework::create(HINSTANCE instance, int cmdShow) -> void
{
  m_framework = new framework(instance, cmdShow);
  m_framework->Init();
  g_rng.seed(static_cast<unsigned int>(performance_counter::QueryValue()));
}

auto framework:: get() -> framework&
{
  return *m_framework;
}

auto framework::windowData() -> window_data&
{
  return get().m_windowData;
}

auto framework::swapChain() -> winrt::com_ptr<IDXGISwapChain>&
{
  return get().m_swapChain;
}

auto framework::renderTarget() -> winrt::com_ptr<ID2D1RenderTarget>&
{
  return get().m_renderTarget;
}

auto framework::dwriteFactory() -> winrt::com_ptr<IDWriteFactory>&
{
  return get().m_dwriteFactory;
}

auto framework::directSound() -> winrt::com_ptr<IDirectSound8>&
{
  return get().m_directSound;
}

auto framework::primarySoundBuffer() -> winrt::com_ptr<IDirectSoundBuffer>&
{
  return get().m_primarySoundBuffer;
}

auto framework::keyboard() -> winrt::com_ptr<IDirectInputDevice8>&
{
  return get().m_keyboard;
}

auto framework::fps() -> int
{
  return 60;
}

auto framework::rng() -> std::mt19937&
{
  return g_rng;
}

framework::framework(HINSTANCE instance, int cmdShow) : m_instance(instance), m_cmdShow(cmdShow)
{
}

auto framework::Init() -> void
{
  RegisterMainWindowClass(m_instance);
  m_window = CreateMainWindow(m_instance, m_cmdShow, m_windowData);
  m_swapChain = CreateSwapChain(m_window, framework::fps(), 1);
  m_renderTarget = CreateRenderTarget(m_swapChain.get());
  m_dwriteFactory = CreateDWriteFactory();
  m_directSound = CreateDirectSound(m_window);
  m_primarySoundBuffer = CreatePrimarySoundBuffer(m_directSound.get());
  m_keyboard = CreateKeyboard(m_instance, m_window);

  HRESULT hr = m_swapChain->SetFullscreenState(FALSE, NULL);
  if( FAILED(hr) ) 
    throw std::exception();
}
