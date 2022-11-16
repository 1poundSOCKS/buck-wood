#include "gtest/gtest.h"
#include "game_math.h"
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

  game_shape shape1(s1_points, s1_pointCount), shape2(s2_points, s2_pointCount);

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

  game_shape shape1(s1_points, s1_pointCount), shape2(s2_points, s2_pointCount);

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

  game_shape shape1(s1_points, s1_pointCount), shape2(s2_points, s2_pointCount);

  EXPECT_EQ(PointsInside(shape1.points, shape2), false);
  EXPECT_EQ(PointsInside(shape2.points, shape1), false);
}

TEST(PointInside,Test1)
{
  const game_point points[] = {
    game_point(20, 20),
    game_point(1980, 20),
    game_point(1980, 980),
    game_point(1200, 900),
    game_point(800, 700),
    game_point(20, 980)
  };

  const int pointCount = sizeof(points) / sizeof(game_point);
  
  game_shape shape(points, pointCount);

  game_point point(500.0f,500.0f);

  EXPECT_EQ(PointInside(point,shape),true);
}

TEST(GameLevelFilename,Test1)
{
  game_level_data_filenames gameLevelFilenames;
  std::wstring filename = gameLevelFilenames.GetNext();
  EXPECT_STREQ(filename.c_str(),L"level_001.json");
  filename = gameLevelFilenames.GetNext();
  EXPECT_STREQ(filename.c_str(),L"level_002.json");
  filename = gameLevelFilenames.GetNext();
  EXPECT_STREQ(filename.c_str(),L"level_003.json");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
