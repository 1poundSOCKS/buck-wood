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
    std::string { "         0000" },
    std::string { "         0   " },
    std::string { "         0 G " },
    std::string { "        00 3 " },
    std::string { "    000000 2 " },
    std::string { "    0        " },
    std::string { "    0 3  0000" },
    std::string { "    0    0000" },
    std::string { "      3      " },
    std::string { "   0     2   " },
    std::string { "   0    00   " },
    std::string { "   0    00   " },
    std::string { " E 0  P      " },
    std::string { "   0     1   " },
    std::string { "0000000000000" }
  };

};

inline level_1::level_1() : level_base { m_levelData }
{
}

inline level_1::~level_1()
{
}
