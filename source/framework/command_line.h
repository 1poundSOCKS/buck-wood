#pragma once

class command_line
{

public:

  static auto create(LPCWSTR cmdLine) -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto contains(LPCWSTR arg) -> bool;

private:

  command_line(LPCWSTR cmdLine);
  [[nodiscard]] auto Contains(LPCWSTR arg) const -> bool;

private:

  inline static command_line* m_instance { nullptr };
  std::list<std::wstring> m_args;

};
