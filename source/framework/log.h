#pragma once

class log
{

public:

  enum class type { info, error, fatal };

  static auto create() -> void;
  static auto destroy() -> void;
  static auto open() -> void;
  static auto file() -> std::ofstream&;
  template <typename...Args> static auto write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void;

private:

  auto Open() -> void;
  template <typename...Args> auto Write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void;

private:

  inline static log* m_instance { nullptr };
  std::ofstream m_file;

};

template <typename...Args> auto log::write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void
{
  m_instance->Write(msgType, fmt, std::forward<Args>(args)...);
}

template <typename...Args> auto log::Write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void
{
  m_file << std::format(fmt, std::forward<Args>(args)...).c_str() << "\n";
}
