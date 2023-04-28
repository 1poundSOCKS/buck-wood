#include "pch.h"
#include "menu.h"

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
}

auto menu::Render(D2D1_RECT_F viewRect) const -> void
{
  for( const auto& button : m_buttons )
  {
    button.Render(viewRect);
  }
}
