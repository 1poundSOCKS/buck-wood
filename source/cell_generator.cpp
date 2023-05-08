#include "pch.h"
#include "cell_generator.h"
#include "perlin_simplex_noise.h"

cell_generator::cell_generator(int columnCount, int rowCount) : m_columnCount(columnCount), m_rowCount(rowCount)
{

}

auto cell_generator::Get(std::back_insert_iterator<collection> inserter, std::function<bool(float)> noiseValueCheck) const -> void
{
  for( int column = 0; column < m_columnCount; ++column )
  {
    for( int row = 0; row < m_rowCount; ++row )
    {
      if( column != 0 && row != 0)
      {
        auto noise = psn::GetNoise(static_cast<float>(column), static_cast<float>(row));

        if( noiseValueCheck(noise) )
        {
          inserter = { column, row };
        }
      }
    }
  }
}
