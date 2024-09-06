#pragma once

class pixel_geometry
{

public:

  pixel_geometry();
  [[nodiscard]] operator winrt::com_ptr<ID2D1Geometry>() const;

private:

  winrt::com_ptr<ID2D1Geometry> m_geometry;

};
