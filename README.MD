# ⚙️ CNC Virtual Simulator

> A real-time CNC machine simulator built in C++17.

The CNC Virtual Simulator parses industry-standard G-code programs and simulates the motion of a 3-axis CNC machine in real time; detecting errors, logging telemetry, and producing toolpath reports. Built to mirror the kind of low-level, performance-critical software used in real manufacturing and robotics systems.

---

## Features

- **G-code parser** — tokenises and validates G-code programs (G0, G1, G2, G3, M-codes)
- **Motion engine** — computes linear and arc toolpaths with feed rate handling
- **Real-time simulation loop** — producer-consumer architecture using C++ threads
- **Telemetry logger** — records position, feed rate, and machine state at every step
- **Error detection** — catches overtravel, invalid commands, and feed rate violations
- **Machine configuration** — fully configurable axis limits and parameters via JSON
- **Toolpath report** — outputs a CSV of the full toolpath after each run

---

## Why I Built This

CNC machines, robotic arms, and automated manufacturing systems all share the same core software challenge: translating a high-level instruction (move here, at this speed) into precise, real-time machine control. This project implements that pipeline from scratch; G-code parsing, motion interpolation, real-time threading, and structured logging using only C++17 and the standard library.

It targets the same problem domain as industrial motion controllers used in CNC mills, 3D printers, and robotic end-effectors.

---

## Tech Stack

| Tool | Purpose |
|---|---|
| C++17 | Core application language |
| CMake 3.20+ | Build system |
| nlohmann/json | Machine configuration parsing |
| Google Test | Unit testing |
| STL (`variant`, `optional`, `thread`, `chrono`) | Modern C++ features throughout |

---

## Getting Started

### Prerequisites

- GCC 11+ or Clang 14+
- CMake 3.20+
- Git

### Build

```bash
git clone https://github.com/louisnguyenn/cnc-virtual-sim.git
cd cnc-virtual-sim
mkdir build && cd build
cmake ..
cmake --build .
```

### Run

```bash
./cnc_simulator path/to/program.gcode
```

### Run Tests

```bash
cd build
ctest --output-on-failure
```

---

## Usage

Pass a G-code file as an argument:

```bash
./cnc_simulator tests/programs/square.gcode
```

Example output:

```
CNC Simulator v0.1.0
Parsed 12 commands from square.gcode
[INFO] Simulation started
[INFO] LinearMove → X:50.00 Y:0.00  F:300
[INFO] LinearMove → X:50.00 Y:50.00 F:300
[INFO] LinearMove → X:0.00  Y:50.00 F:300
[INFO] LinearMove → X:0.00  Y:0.00  F:300
[INFO] Program end
Cycle time : 4.2s
Total distance: 200.0mm
Toolpath saved to toolpath.csv
```

---

## Key Concepts Demonstrated

- **std::variant** — type-safe representation of different G-code command types
- **std::optional** — safe return values from the parser when a line produces no command
- **std::thread & std::mutex** — producer-consumer simulation loop
- **std::chrono** — real-time motion timing
- **OOP design** — Parser, MotionEngine, Logger as decoupled classes
- **CMake FetchContent** — automatic dependency management
- **Google Test** — unit tests for the parser covering edge cases

---

## Sample G-code Programs

A simple square toolpath (`tests/programs/square.gcode`):

```gcode
G90          ; absolute positioning
G0 X0 Y0 Z5 ; rapid move to start, safe height
G1 Z0 F100   ; plunge down
G1 X50 F300  ; cut right
G1 Y50       ; cut up
G1 X0        ; cut left
G1 Y0        ; cut back to start
G0 Z5        ; retract
M30          ; end program
```

---

## Credits
Louis Nguyen
