#pragma once

#include "framework.h"
#include "level_base.h"

class level_1 : public level_base
{

public:

  level_1();
  ~level_1() override;

private:

  inline static std::array m_levelData { 
    std::string { "00E00" },
    std::string { "  F  " },
    std::string { "     " },
    std::string { "  2  " },
    std::string { "     " },
    std::string { "0   0" },
    std::string { "0   0" },
    std::string { "0   0" },
    std::string { "0 P 0" }
  };

};
