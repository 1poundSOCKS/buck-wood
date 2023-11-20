#pragma once

class render_text
{

public:

  enum class selector { menu_text_default = 0, menu_text_hover, menu_text_small };

  render_text();
  [[nodiscard]] auto get(selector textSelector) const -> const winrt::com_ptr<IDWriteTextFormat>&;

private:

  using text_format_collection = std::vector<winrt::com_ptr<IDWriteTextFormat>>;

  text_format_collection m_textFormat;

};

[[nodiscard]] inline auto render_text::get(selector textSelector) const -> const winrt::com_ptr<IDWriteTextFormat>&
{
  return m_textFormat[static_cast<std::underlying_type<selector>::type>(textSelector)];
}
