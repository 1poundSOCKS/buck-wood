#include "log.h"

namespace com_logger
{
  template <typename...Args> static auto write(HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void;
  template <typename...Args> static auto fatal(HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void;
};

template <typename...Args> auto com_logger::write(HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void
{
  log::write(SUCCEEDED(hr) ? log::type::info : log::type::error, fmt, std::forward<Args>(args)...);
}

template <typename...Args> auto com_logger::fatal(HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void
{
  log::write(SUCCEEDED(hr) ? log::type::info : log::type::fatal, fmt, std::forward<Args>(args)...);
}
