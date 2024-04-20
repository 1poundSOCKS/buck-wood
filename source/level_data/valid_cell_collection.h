#pragma once

using valid_cell = std::tuple<bool, int, int, POINT_2F, winrt::com_ptr<ID2D1TransformedGeometry>>;
using valid_cell_collection = std::vector<valid_cell>;
