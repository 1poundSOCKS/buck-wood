#pragma once

class log
{

public:

  static auto create() -> void;
  static auto destroy() -> void;
  static auto open() -> void;
  static auto file() -> std::ofstream&;

  auto operator<<(const char* output) -> std::ostream&;

private:

  auto Open() -> void;

private:

  inline static log* m_instance { nullptr };
  std::ofstream m_file;

};
