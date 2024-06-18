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
    std::string { "             " },
    std::string { " 2  1  2  1  " },
    std::string { "  E          " },
    std::string { "            2" },
    std::string { "0000000000   " },
    std::string { "         0   " },
    std::string { "  P      0   " },
    std::string { "         0   " },
    std::string { "             " },
    std::string { "             " }
  };

};

inline level_2::level_2() : level_base { m_levelData }
{
}

inline level_2::~level_2()
{
}
