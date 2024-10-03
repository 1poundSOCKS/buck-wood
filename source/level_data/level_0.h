#pragma once

#include "framework.h"
#include "level_base.h"

class level_0 : public level_base
{

public:

  level_0();
  ~level_0() override;

private:

 inline static std::array m_levelData { 
    std::string { "                             " },
    std::string { "                             " },
    std::string { "      1                      " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "                         3   " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "              P              " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "         2                   " },
    std::string { "                             " },
    std::string { "                             " },
    std::string { "                             " }
 };

};

inline level_0::level_0() : level_base { m_levelData }
{
}

inline level_0::~level_0()
{
}
