#pragma once

#include "framework.h"
#include "level_base.h"

class level_2 : public level_base
{

public:

  level_2();
  ~level_2() override;

private:

  inline static std::array m_levelData { 
    std::string { "0000000000 2 " },
    std::string { "0000000000   " },
    std::string { "00000        " },
    std::string { "00000    0000" },
    std::string { "00000    0000" },
    std::string { "             " },
    std::string { "  00     2   " },
    std::string { "  00    00   " },
    std::string { "  00    00   " },
    std::string { "  00         " },
    std::string { "P 00     1   " }
  };

};
