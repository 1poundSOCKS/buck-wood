#include "pch.h"
#include "menu.h"

auto menu::SetCallbackForHiddenFlag(callback_for_hidden_flag callbackForHiddenFlag) -> void
{
  m_callbackForHiddenFlag = callbackForHiddenFlag;
}

auto menu::AddButton(button&& menuButton) -> void
{
  m_buttons.emplace_back(menuButton);
}

auto menu::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  for( auto& button : m_buttons )
  {
    button.Initialize(renderTarget);
  }
}

auto menu::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  for( auto& button : m_buttons )
  {
    button.Update(inputData, clockCount);
  }

  m_hidden = m_callbackForHiddenFlag();
}

auto menu::Render(D2D1_RECT_F viewRect) const -> void
{
  if( !m_hidden )
  {
    for( const auto& button : m_buttons )
    {
      if( !button.GetHoverState() )
        button.Render(viewRect);
    }

    for( const auto& button : m_buttons )
    {
      if( button.GetHoverState() )
        button.Render(viewRect);
    }
  }
}
