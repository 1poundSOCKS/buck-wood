#pragma once

class log
{

public:

  static auto create() -> void;
  static auto destroy() -> void;
  static auto open() -> void;
  static auto file() -> std::ofstream&;
  template <typename...Args> static auto message(std::format_string<Args...> fmt, Args&&... args) -> void;

private:

  auto Open() -> void;
  template <typename...Args> auto Message(std::format_string<Args...> fmt, Args&&... args) -> void;

private:

  inline static log* m_instance { nullptr };
  std::ofstream m_file;

};

template <typename...Args> auto log::message(std::format_string<Args...> fmt, Args&&... args) -> void
{
  m_instance->Message(fmt, std::forward<Args>(args)...);
}

template <typename...Args> auto log::Message(std::format_string<Args...> fmt, Args&&... args) -> void
{
  m_file << std::format(fmt, std::forward<Args>(args)...).c_str() << "\n";
}
