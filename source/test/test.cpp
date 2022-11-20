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
