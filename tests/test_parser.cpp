#include "parser/GcodeParser.h"
#include <gtest/gtest.h>

// linear move tests
TEST(ParserTest, G1LinearMove)
{
    GcodeParser parser;

    auto cmd = parser.parseLine("G1 X10.0 Y5.5 F300");
    ASSERT_TRUE(cmd.has_value());
    ASSERT_TRUE(std::holds_alternative<LinearMove>(*cmd));

    auto &mv = std::get<LinearMove>(*cmd);
    EXPECT_DOUBLE_EQ(mv.x, 10.0);
    EXPECT_DOUBLE_EQ(mv.y, 5.5);
    EXPECT_DOUBLE_EQ(mv.feedrate, 300.0);
    EXPECT_FALSE(mv.rapid);
}
