#include "pch.h"
#include "menu_renderer.h"
#include "column_def.h"
#include "row_def.h"
#include "text_renderer.h"
#include "render_target.h"

auto menu_renderer::Write(const menu_item& menuItem) const -> void
{
  struct visitor
  {
    visitor(const menu_renderer* menuRenderer) : m_this { menuRenderer }
    {
    }

    void operator()(const button& item)
    {
      m_this->Write(item);
    }

    void operator()(const setting_slider& item)
    {
      m_this->Write(item);
    }

    const menu_renderer* m_this { nullptr };
  };

  std::visit(visitor { this }, menuItem.Get());
}

auto menu_renderer::Write(const button& buttonObject) const -> void
{
  const auto& rect = buttonObject.Rect();
  const auto& text = buttonObject.Text();
  
  if( buttonObject.HoverState() )
  {
    text_renderer textRenderer { m_textHoverBrush, m_hoverText };
    textRenderer.Write(rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, text.c_str());
  }
  else
  {
    text_renderer textRenderer { m_textBrush, m_defaultText };
    textRenderer.Write(rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, text.c_str());
  }
}

auto menu_renderer::Write(const setting_slider& settingSlider) const -> void
{
  const auto& rect = settingSlider.Rect();

  column_def columnDef { rect, 2 };

  auto headerRect = columnDef[0];

  auto textBrush = settingSlider.HoverState() ? m_textHoverBrush : m_textBrush;

  text_renderer textRenderer { { textBrush }, { m_smallText } };
  textRenderer.Write(headerRect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, settingSlider.Name().c_str());

  auto sliderRect = columnDef[1];

  row_def rowDef { sliderRect, static_cast<size_t>(settingSlider.Max() - settingSlider.Min())  };

  auto currentRow = settingSlider.Min();

  for( const auto& rowRect : rowDef )
  {
    if( currentRow++ < settingSlider.Value() )
    {
      render_target::renderTarget()->FillRectangle(rowRect, m_buttonHoverBrush.get());
    }
    else
    {
      render_target::renderTarget()->FillRectangle(rowRect, m_buttonBrush.get());
    }
  }
}
