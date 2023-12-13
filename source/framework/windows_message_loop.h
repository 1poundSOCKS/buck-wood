#pragma once

class windows_message_loop
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static auto run(auto&& callable) -> void;

private:

  windows_message_loop() = default;
  auto ProcessWindowMessages() -> bool;

private:

  inline static windows_message_loop* m_instance { nullptr };
  bool m_continue { true };

};

inline auto windows_message_loop::create() -> void
{
  destroy();
  m_instance = new windows_message_loop();
}

inline auto windows_message_loop::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

inline auto windows_message_loop::run(auto&& callable) -> void
{
  while( m_instance->ProcessWindowMessages() && callable() ) {}
}
