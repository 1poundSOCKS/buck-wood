#pragma once

#include "game_score.h"
#include "framework.h"

class game_score_renderer
{

public:

  auto Write(const game_score& gameScore) const -> void
  {
    std::wstring score = std::format(L"{:05d}", gameScore.Value());
    const auto& textRenderer = GetTextRenderer(gameScore);
    auto rect = GetRect(gameScore);
    auto textAlignment = GetTextAlignment(gameScore);
    textRenderer.Write(rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, textAlignment, score.c_str());
  }

private:

  [[nodiscard]] auto GetRect(const game_score& gameScore) const -> D2D1_RECT_F
  {    
    switch( gameScore.Type() )
    {
      case game_score::value_type::total:
        return m_rect;
      case game_score::value_type::power_ups:
        return m_powerUpsRect;
      default:
        return m_rect;
    }
  }

  [[nodiscard]] auto GetTextAlignment(const game_score& gameScore) const -> DWRITE_TEXT_ALIGNMENT
  {    
    switch( gameScore.Type() )
    {
      case game_score::value_type::total:
        return DWRITE_TEXT_ALIGNMENT_TRAILING;
      case game_score::value_type::power_ups:
        return DWRITE_TEXT_ALIGNMENT_LEADING;
      default:
        return DWRITE_TEXT_ALIGNMENT_CENTER;
    }
  }

  [[nodiscard]] auto GetTextRenderer(const game_score& gameScore) const -> const text_renderer&
  {
    switch( gameScore.Type() )
    {
      case game_score::value_type::total:
        return m_renderTotal;
      case game_score::value_type::power_ups:
        return m_renderPowerUps;
      default:
        return m_renderTotal;
    }
  }

  D2D1_SIZE_F m_renderTargetSize { render_target::get()->GetSize() };
  D2D1_RECT_F m_rect { 0, 0, m_renderTargetSize.width / 2 - 100, 50 };
  D2D1_RECT_F m_powerUpsRect { m_renderTargetSize.width / 2 + 100, 0, m_renderTargetSize.width - 1, 50 };
  render_brush m_brushTotal { screen_render_brush_blue.CreateBrush() };
  render_brush m_brushPowerUps { screen_render_brush_cyan.CreateBrush() };
  render_text_format m_defaultText { render_text_format_def { L"Courier New", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50 }.CreateTextFormat() };
  text_renderer m_renderTotal { m_brushTotal, m_defaultText };
  text_renderer m_renderPowerUps { m_brushPowerUps, m_defaultText };

};
