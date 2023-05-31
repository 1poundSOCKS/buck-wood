#include "pch.h"
#include "menu.h"

menu::menu()
{
}

menu::menu(const menu& menuToCopy)
{
  m_callbackForHiddenFlag = menuToCopy.m_callbackForHiddenFlag;
  m_hidden = menuToCopy.m_hidden;
  m_buttons = menuToCopy.m_buttons;
  UpdateAllButtonCallbacks();
}

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
    UpdateButtonCallbacks(button);
  }
}

auto menu::Update(const object_input_data& inputData) -> void
{
  m_hidden = m_callbackForHiddenFlag();

  for( auto& button : m_buttons )
  {
    button.Update(inputData);
  }
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

auto menu::UpdateAllButtonCallbacks() -> void
{
  for( auto& button : m_buttons )
  {
    UpdateButtonCallbacks(button);
  }
}

auto menu::UpdateButtonCallbacks(button& buttonToUpdate) -> void
{
  buttonToUpdate.SetCallbackForEnabledFlag([this]() -> bool
  {
    return !m_hidden;
  });
}
