#pragma once
#include <optional>
#include <variant>

// g-code structure

/*
"G1 X50 Y0 F300"   →   parse it   →   LinearMove { x=50, y=0, z=0, feedrate=300 }
"G2 X0 Y50 I-25"   →   parse it   →   ArcMove { x=0, y=50, i=-25, j=0, cw=true }
"M30"              →   parse it   →   ProgramEnd {}
*/

struct LinearMove
{
    double x, y, z; // position
    double feedrate;
    bool rapid; // G0 = true (fast), G1 = false (controlled)
};

struct ArcMove
{
    double x, y, z; // position
    double i, j, k; // offset to arc centre
    double feedrate;
    bool clockwise; // G2 = true, G3 = false (counterclockwise)
};

struct SpindleCmd
{
    bool on;    // M3 - spindle on, M5 - spindle off
    int rpm;    // default - 1000
};

struct DwellCmd
{
    double seconds; // G4 P__ - pause for 'x' amount of seconds
};

struct ProgramEnd
{
    // M30 - end of program
};

// setting GCommand as an alias
// could be ANY of the types (GCode)

using GCommand = std::variant<LinearMove, ArcMove, SpindleCmd, DwellCmd, ProgramEnd>;
