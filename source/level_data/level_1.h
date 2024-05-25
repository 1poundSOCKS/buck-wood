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
    std::string { "00000" },
    std::string { "     " },
    std::string { "  E  " },
    std::string { "  2  " },
    std::string { "     " },
    std::string { "0   0" },
    std::string { "0   0" },
    std::string { "0 P 0" },
    std::string { "0   0" }
  };

};

inline level_1::level_1() : level_base { m_levelData }
{
}

inline level_1::~level_1()
{
}
