#include "pch.h"
#include "log.h"

auto log::create() -> void
{
  destroy();

  m_instance = new log();
}

auto log::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto log::open() -> void
{
  m_instance->Open();
}

auto log::file() -> std::ofstream&
{
  return m_instance->m_file;
}

auto log::Open() -> void
{
  m_file.open("log.txt");
}
