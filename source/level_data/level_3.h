#pragma once

#include "framework.h"
#include "level_base.h"

class level_3 : public level_base
{

public:

  level_3();
  ~level_3() override;

private:

  inline static std::array m_levelData { 
    std::string { "0000000000000" },
    std::string { " 2  1  2  1  " },
    std::string { "             " },
    std::string { "      P      " },
    std::string { "000000 000000" },
    std::string { "000000H000000" },
    std::string { "000000E000000" }
  };

};

inline level_3::level_3() : level_base { m_levelData }
{
}

inline level_3::~level_3()
{
}
