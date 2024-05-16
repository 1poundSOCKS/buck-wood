#pragma once

#include "framework.h"
#include "level_base.h"
#include "level_boundary.h"
#include "level_cell_collection.h"

class level_1 : public level_base
{

public:

  level_1();
  ~level_1() override;

private:

  inline static std::array m_levelData { 
    std::string { "     " },
    std::string { "  2  " },
    std::string { "     " },
    std::string { "     " },
    std::string { "0   0" },
    std::string { "0   0" },
    std::string { "0   0" },
    std::string { "0 P 0" }
  };

};
