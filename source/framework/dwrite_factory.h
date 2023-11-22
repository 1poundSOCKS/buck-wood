#pragma once

class dwrite_factory
{
public:

  static auto create() -> void;
  static auto destroy() -> void;
  static auto get() -> const winrt::com_ptr<IDWriteFactory>&;

private:
  
  static dwrite_factory* m_instance;

  dwrite_factory();
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;
};
