#include "pch.h"
#include "command_line.h"

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
