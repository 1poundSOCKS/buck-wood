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
    std::string { "                         " },
    std::string { "                         " },
    std::string { "    00000       000000   " },
    std::string { "    0                0   " },
    std::string { "    0    000 000     0   " },
    std::string { "    0    000 000     0   " },
    std::string { "    0    00   00     0   " },
    std::string { "            P            " },
    std::string { "    0    00   00     0   " },
    std::string { "    0    000 000     0   " },
    std::string { "    0    000 000     0   " },
    std::string { "    0                0   " },
    std::string { "    00000       000000   " },
    std::string { "                         " },
    std::string { "                         " }
 };

};

inline level_0::level_0() : level_base { m_levelData }
{
}

inline level_0::~level_0()
{
}
