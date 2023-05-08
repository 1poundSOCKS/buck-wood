#pragma once

class cell_generator
{

public:

  struct cell_id
  {
    int column;
    int row;
  };

  using collection = std::vector<cell_id>;

  cell_generator(int columnCount, int rowCount);

  auto Get(std::back_insert_iterator<collection> inserter) const -> void;

private:

  int m_columnCount = 0;
  int m_rowCount = 0;

};
