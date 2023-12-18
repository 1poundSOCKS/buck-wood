#include "pch.h"
#include "level_asteroid.h"
#include "perlin_simplex_noise.h"
#include "renderers.h"
#include "shape_generator.h"

level_asteroid::level_asteroid(float x, float y, float maxWidth, float maxHeight) : m_position { x, y }
{
}
