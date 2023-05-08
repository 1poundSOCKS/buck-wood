#pragma once

class cell_generator
{

public:

  struct cell_id
  {
    int column;
    int row;

    auto operator<=>(const cell_id& cellId) const = default;
  };

  using collection = std::vector<cell_id>;

  cell_generator(int columnCount, int rowCount);
  
  auto Get(std::back_insert_iterator<collection> trueInserter, std::function<bool(float)> noiseValueCheck) const -> void;
  auto Get(std::back_insert_iterator<collection> trueInserter, std::back_insert_iterator<collection> falseInserter, std::function<bool(float)> noiseValueCheck) const -> void;

private:

  int m_columnCount = 0;
  int m_rowCount = 0;

};
