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
    std::string { "0000000000000000000000000000000" },
    std::string { "00    0000000000000000000000000" },
    std::string { "000                       00000" },
    std::string { "000   00000000000000000   00000" },
    std::string { "0000 00000000000000000000 00000" },
    std::string { "0000 00000000000000000000 00000" },
    std::string { "0000 000000000   00000000 00000" },
    std::string { "0000 000000000 P 00000000 00000" },
    std::string { "0000 000000000   00000000 00000" },
    std::string { "0000 0000000000 000000000 00000" },
    std::string { "0000 0000000000 00000000   0000" },
    std::string { "0000            00000000 E 0000" },
    std::string { "0000000000 0000000000000   0000" },
    std::string { "0000000000               000000" },
    std::string { "0000000000000000000000000000000" }
  };

};

inline level_0::level_0() : level_base { m_levelData }
{
}

inline level_0::~level_0()
{
}
