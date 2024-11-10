#include "pch.h"
#include "play_state_renderer.h"

auto play_state_renderer::Write(const play_state &playState) const noexcept -> void
{
  std::wstring text = std::format(L"{:.2f}", playState.TimeRemaining());
  m_textRenderer.Write(m_rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, text.c_str());
}
