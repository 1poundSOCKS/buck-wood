#pragma once

#include "color_scale.h"

class color_scale_brushes
{

public:

  using brush_ptr = winrt::com_ptr<ID2D1SolidColorBrush>;
  using brush_collection = std::vector<brush_ptr>;

  color_scale_brushes(const color_scale& colorScale);

  [[nodiscard]] auto operator[](float scaleValue) const -> const brush_ptr&;

private:

  brush_collection m_brushes;

};
