#include "pch.h"
#include "user_input.h"

user_input* user_input::m_instance { nullptr };

auto user_input::create() -> void
{
  destroy();
  m_instance = new user_input {};
}

auto user_input::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto user_input::update() -> void
{
  m_instance->Update();
}

user_input::user_input()
{
}

auto user_input::Update() -> void
{
  m_gamepadReader.Update();
}
