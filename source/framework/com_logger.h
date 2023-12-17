#include "log.h"

namespace com_logger
{
  template <typename...Args> static auto write(log::type type, HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void;
  template <typename...Args> static auto fatal(log::type type, HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void;
};

template <typename...Args> auto com_logger::write(log::type type, HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void
{
  log::write(SUCCEEDED(hr) ? type : log::type::error, fmt, std::forward<Args>(args)...);
}

template <typename...Args> auto com_logger::fatal(log::type type, HRESULT hr, std::format_string<Args...> fmt, Args&&... args) -> void
{
  log::write(SUCCEEDED(hr) ? type : log::type::fatal, fmt, std::forward<Args>(args)...);
}
