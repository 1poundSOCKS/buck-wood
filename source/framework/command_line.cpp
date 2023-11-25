#include "pch.h"
#include "command_line.h"

auto command_line::create(LPCWSTR cmdLine) -> void
{
  destroy();
  m_instance = new command_line { cmdLine };
}

auto command_line::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

[[nodiscard]] auto command_line::contains(LPCWSTR arg) -> bool
{
  return m_instance->Contains(arg);
}

command_line::command_line(LPCWSTR cmdLine)
{
  int numArgs { 0 };
  auto args = ::CommandLineToArgvW(cmdLine, &numArgs);

  for( int argIndex = 0; argIndex < numArgs; ++argIndex )
  {
    m_args.emplace_back( std::wstring { args[argIndex] } );
  }
}

[[nodiscard]] auto command_line::Contains(LPCWSTR arg) const -> bool
{
  auto found { false };

  for( const auto& currentArg : m_args )
  {
    if( currentArg.compare(arg) == 0 )
    {
      found = true;
    }
  }

  return found;
}
