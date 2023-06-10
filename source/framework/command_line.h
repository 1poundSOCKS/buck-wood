#pragma once

class command_line
{

public:

  command_line(LPCWSTR cmdLine);
  [[nodiscard]] auto Contains(LPCWSTR arg) const -> bool;

private:

  std::list<std::wstring> m_args;

};
