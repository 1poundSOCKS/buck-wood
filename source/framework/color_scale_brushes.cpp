#include "pch.h"
#include "color_scale_brushes.h"
#include "framework.h"
#include "render_brush_def.h"

color_scale_brushes::color_scale_brushes(const color_scale& colorScale)
{
  const auto& renderTarget = render_target::renderTarget();

  auto brushesView = colorScale | std::ranges::views::transform([&renderTarget](auto color) -> brush_ptr
  {
    render_brush_def brushDef { color };
    return brushDef.CreateBrush(renderTarget.get());
  });

  std::ranges::copy(std::cbegin(brushesView), std::cend(brushesView), std::back_inserter(m_brushes));
}

[[nodiscard]] auto color_scale_brushes::operator[](float scaleValue) const -> const brush_ptr&
{
  auto brushIndex = static_cast<int>(static_cast<float>(m_brushes.size() - 1) * scaleValue + 0.5f);
  assert(brushIndex >=0 && brushIndex < m_brushes.size());
  return m_brushes[brushIndex];
}
