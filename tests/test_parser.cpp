#include "parser/GcodeParser.h"
#include <gtest/gtest.h>

// ── Linear Move Tests ──────────────────────────────────────────────

TEST(ParserTest, ParsesG1LinearMove)
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

TEST(ParserTest, ParsesG0RapidMove)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("G0 X0 Y0 Z5");
    ASSERT_TRUE(cmd.has_value());
    auto &mv = std::get<LinearMove>(*cmd);
    EXPECT_TRUE(mv.rapid);
    EXPECT_DOUBLE_EQ(mv.z, 5.0);
}

TEST(ParserTest, NegativeCoordinates)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("G1 X-10.0 Y-20.5 F200");
    ASSERT_TRUE(cmd.has_value());
    auto &mv = std::get<LinearMove>(*cmd);
    EXPECT_DOUBLE_EQ(mv.x, -10.0);
    EXPECT_DOUBLE_EQ(mv.y, -20.5);
}

// ── Modal Feed Rate Tests ──────────────────────────────────────────

TEST(ParserTest, RemembersFeedrateAcrossLines)
{
    GcodeParser parser;
    parser.parseLine("G1 X0 Y0 F500");         // sets feedrate to 500
    auto cmd = parser.parseLine("G1 X10 Y10"); // no F — should use 500
    ASSERT_TRUE(cmd.has_value());
    auto &mv = std::get<LinearMove>(*cmd);
    EXPECT_DOUBLE_EQ(mv.feedrate, 500.0);
}

// ── Arc Move Tests ─────────────────────────────────────────────────

TEST(ParserTest, ParsesG2ClockwiseArc)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("G2 X10 Y0 I5 J0 F300");
    ASSERT_TRUE(cmd.has_value());
    ASSERT_TRUE(std::holds_alternative<ArcMove>(*cmd));
    auto &arc = std::get<ArcMove>(*cmd);
    EXPECT_TRUE(arc.clockwise);
    EXPECT_DOUBLE_EQ(arc.i, 5.0);
}

TEST(ParserTest, ParsesG3CounterClockwiseArc)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("G3 X10 Y0 I5 J0 F300");
    ASSERT_TRUE(cmd.has_value());
    auto &arc = std::get<ArcMove>(*cmd);
    EXPECT_FALSE(arc.clockwise);
}

// ── Blank & Comment Tests ──────────────────────────────────────────

TEST(ParserTest, BlankLineReturnsEmpty)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("   ");
    EXPECT_FALSE(cmd.has_value());
}

TEST(ParserTest, CommentLineReturnsEmpty)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("; this is a comment");
    EXPECT_FALSE(cmd.has_value());
}

TEST(ParserTest, InlineCommentStripped)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("G1 X10 Y5 F300 ; move to start");
    ASSERT_TRUE(cmd.has_value());
    auto &mv = std::get<LinearMove>(*cmd);
    EXPECT_DOUBLE_EQ(mv.x, 10.0);
}

// ── M-code Tests ───────────────────────────────────────────────────

TEST(ParserTest, ParsesM30ProgramEnd)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("M30");
    ASSERT_TRUE(cmd.has_value());
    EXPECT_TRUE(std::holds_alternative<ProgramEnd>(*cmd));
}

TEST(ParserTest, ParsesM3SpindleOn)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("M3 S1000");
    ASSERT_TRUE(cmd.has_value());
    ASSERT_TRUE(std::holds_alternative<SpindleCmd>(*cmd));
    auto &sp = std::get<SpindleCmd>(*cmd);
    EXPECT_TRUE(sp.on);
    EXPECT_EQ(sp.rpm, 1000);
}

TEST(ParserTest, ParsesM5SpindleOff)
{
    GcodeParser parser;
    auto cmd = parser.parseLine("M5");
    ASSERT_TRUE(cmd.has_value());
    auto &sp = std::get<SpindleCmd>(*cmd);
    EXPECT_FALSE(sp.on);
}

// ── Modal State Tests ──────────────────────────────────────────────

TEST(ParserTest, G90G91DoNotReturnCommand)
{
    GcodeParser parser;
    auto cmd90 = parser.parseLine("G90");
    auto cmd91 = parser.parseLine("G91");
    EXPECT_FALSE(cmd90.has_value());
    EXPECT_FALSE(cmd91.has_value());
}

// ── File Parsing Test ──────────────────────────────────────────────

TEST(ParserTest, ParsesSquareGcodeFile)
{
    GcodeParser parser;
    // Adjust path if needed depending on where you run tests from
    auto cmds = parser.parseFile("../tests/programs/square.gcode");
    // square.gcode has 7 motion commands (G0, G1x5, G0) + M30 = 8 total
    EXPECT_EQ(cmds.size(), 8);
    // First command should be a rapid move (G0)
    EXPECT_TRUE(std::holds_alternative<LinearMove>(cmds[0]));
    auto &first = std::get<LinearMove>(cmds[0]);
    EXPECT_TRUE(first.rapid);
    // Last command should be program end
    EXPECT_TRUE(std::holds_alternative<ProgramEnd>(cmds.back()));
}