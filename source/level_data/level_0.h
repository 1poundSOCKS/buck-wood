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
    std::string { "         0000000000000         " },
    std::string { "       000           000       " },
    std::string { "     000               000     " },
    std::string { "     000               000     " },
    std::string { "   000                   000   " },
    std::string { "   000                   000   " },
    std::string { " 000                       000 " },
    std::string { "00             P             00" },
    std::string { "00                           00" },
    std::string { "00             E             00" },
    std::string { " 000                       000  "},
    std::string { "   000                   000   " },
    std::string { "   000                   000   " },
    std::string { "     000               000     " },
    std::string { "     000               000     " },
    std::string { "       000           000       " },
    std::string { "         0000000000000         " }
  };

};

inline level_0::level_0() : level_base { m_levelData }
{
}

inline level_0::~level_0()
{
}
