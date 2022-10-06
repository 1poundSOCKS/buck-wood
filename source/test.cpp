#include "gtest/gtest.h"
#include "math.h"
#include "game_objects.h"

TEST(angle,0)
{
  EXPECT_EQ(CalculateAngle(0,0,0,-1), 0);
}

TEST(angle,0_alt)
{
  EXPECT_EQ(CalculateAngle(1,1,1,-2), 0);
}

TEST(angle,45)
{
  EXPECT_EQ(CalculateAngle(0,0,1,-1), 45);
}

TEST(angle,90)
{
  EXPECT_EQ(CalculateAngle(0,0,1,0), 90);
}

TEST(angle,135)
{
  EXPECT_EQ(CalculateAngle(0,0,1,1), 135);
}

TEST(angle,180)
{
  EXPECT_EQ(CalculateAngle(0,0,0,1), 180);
}

TEST(angle,225)
{
  EXPECT_EQ(CalculateAngle(0,0,-1,1), 225);
}

TEST(angle,270)
{
  EXPECT_EQ(CalculateAngle(0,0,-1,0), 270);
}

TEST(ShapeInside,noOverlap)
{
  const game_point s1_points[] = {
    game_point(0, 0),
    game_point(10, 10),
    game_point(0, 10)
  };

  const game_point s2_points[] = {
    game_point(20, 20),
    game_point(30, 30),
    game_point(20, 20)
  };

  const int s1_pointCount = sizeof(s1_points) / sizeof(game_point);
  const int s2_pointCount = sizeof(s2_points) / sizeof(game_point);

  game_shape shape1, shape2;

  InitializeShape(s1_points, s1_pointCount, shape1);
  InitializeShape(s2_points, s2_pointCount, shape2);

  EXPECT_EQ(PointsInside(shape1.points, shape2), false);
  EXPECT_EQ(PointsInside(shape2.points, shape1), false);
}

TEST(ShapeInside,fully)
{
  const game_point s1_points[] = {
    game_point(0, 0),
    game_point(100, 0),
    game_point(100, 100),
    game_point(0, 100)
  };

  const game_point s2_points[] = {
    game_point(10, 10),
    game_point(20, 20),
    game_point(10, 20)
  };

  const int s1_pointCount = sizeof(s1_points) / sizeof(game_point);
  const int s2_pointCount = sizeof(s2_points) / sizeof(game_point);

  game_shape shape1, shape2;

  InitializeShape(s1_points, s1_pointCount, shape1);
  InitializeShape(s2_points, s2_pointCount, shape2);

  EXPECT_EQ(PointsInside(shape1.points, shape2), false);
  EXPECT_EQ(PointsInside(shape2.points, shape1), true);
}

TEST(ShapeInside,partially)
{
  const game_point s1_points[] = {
    game_point(0, 0),
    game_point(100, 100),
    game_point(0, 100)
  };

  const game_point s2_points[] = {
    game_point(20, 50),
    game_point(20, 200),
    game_point(20, 200)
  };

  const int s1_pointCount = sizeof(s1_points) / sizeof(game_point);
  const int s2_pointCount = sizeof(s2_points) / sizeof(game_point);

  game_shape shape1, shape2;

  InitializeShape(s1_points, s1_pointCount, shape1);
  InitializeShape(s2_points, s2_pointCount, shape2);

  EXPECT_EQ(PointsInside(shape1.points, shape2), false);
  EXPECT_EQ(PointsInside(shape2.points, shape1), false);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
