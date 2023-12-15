#pragma once

class log
{

public:

  enum class type { info, error, fatal };

  static auto create() -> void;
  static auto destroy() -> void;

  template <typename...Args> static auto write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void;

private:

  auto Open() -> void;
  template <typename...Args> auto Write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void;
  static auto GetMessageTypeAsString(type msgType) -> const char*;

private:

  inline static log* m_instance { nullptr };
  std::ofstream m_file;
  std::chrono::system_clock::time_point m_creationTime { std::chrono::system_clock::now() };

};

template <typename...Args> auto log::write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void
{
  m_instance->Write(msgType, fmt, std::forward<Args>(args)...);
}

template <typename...Args> auto log::Write(type msgType, std::format_string<Args...> fmt, Args&&... args) -> void
{ 
  auto timestamp = std::chrono::system_clock::now() - m_creationTime;
  auto msg = std::format(fmt, std::forward<Args>(args)...);

  m_file << std::format("{} {} {}", timestamp.count(), GetMessageTypeAsString(msgType), msg) << "\n";

  if( msgType == type::fatal )
  {
    m_file.flush();
    exit(1);
  }
}

inline auto log::GetMessageTypeAsString(type msgType) -> const char*
{
  switch( msgType )
  {
    case type::info:
      return "INFO";
    case type::error:
      return "ERROR";
    case type::fatal:
      return "FATAL";
    default:
      return "***";
  }
}
