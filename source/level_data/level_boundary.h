#pragma once

#include "geometry_builder.h"
#include "boundary_build_command.h"

class level_boundary
{

public:

  using build_type = std::vector<boundary_build_command>;
  using build_type_collection = std::list<build_type>;
  using points_type = std::vector<geometry_builder::point>;

  auto AddGeometry(std::ranges::input_range auto&& commands) -> void
  {
    m_builds.emplace_back(commands);    
  }

  auto Build(auto outputPoints) -> void
  {
    Build(std::begin(m_builds), outputPoints);
  }

private:

  auto Build(build_type_collection::const_iterator currentBuild, auto outputPoints) -> void
  {
    for( const auto& inputCommand : *currentBuild )
    {
      switch( inputCommand.Type() )
      {
        case boundary_build_command::type::portal:
          if( std::next(currentBuild) != std::end(m_builds) )
          {
            Build(std::next(currentBuild), outputPoints);
          }
          break;
        default:
          m_builder.Run(inputCommand, outputPoints);
          break;
      }
    }
  }

  build_type_collection m_builds;
  geometry_builder m_builder;

};
