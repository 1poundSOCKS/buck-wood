#include "pch.h"
#include "log.h"

auto log::create() -> void
{
  destroy();
  m_instance = new log();
  m_instance->Open();
}

auto log::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto log::Open() -> void
{
  m_file.open("log.txt");
  write(type::info, "log created - {}", m_creationTime);
}
