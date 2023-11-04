#include "pch.h"
#include "render_text.h"
#include "render_text_format_def.h"
#include "dwrite_factory.h"

render_text::render_text()
{
  using format_def_entry = std::tuple<selector, render_text_format_def>;

  auto renderTextFormats = std::array
  {
    format_def_entry { selector::menu_text_default, render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100 } },
    format_def_entry { selector::menu_text_hover, render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100 } }
  };

  const auto& dwriteFactory = dwrite_factory::get();

  for( const auto& [index, format] : renderTextFormats )
  {
    size_t indexValue = static_cast<std::underlying_type<selector>::type>(index);
    auto requiredSize = max(indexValue + 1, m_textFormat.size());
    m_textFormat.resize(requiredSize);
    m_textFormat[indexValue] = format.CreateTextFormat(dwriteFactory.get());
  }
}
