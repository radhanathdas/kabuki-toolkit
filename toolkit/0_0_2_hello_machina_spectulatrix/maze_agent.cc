/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    //seam_0_0/0_0_0_experiments/maze_agent.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "maze_agent.h"

#include <conio.h>
#include <windows.h>
#include <iostream>
using namespace std;

namespace _ {

KeyControl::KeyControl(VKCode code, const char* label)
    : code(code), label(label) {}

void KeyControl::Reprogram() { PrintLn('\n'); }

//------------------------------------------------------------------------------

const char* XYDirectionAcronyms() {
  // Direction acronyms aligned to a 4-bit boundary.
  static const char kDirectionAcronyms[] = {
      'R', 0,   0, 0,  //
      'U', 'R', 0, 0,  //
      'U', 0,   0, 0,  //
      'U', 'L', 0, 0,  //
      'L', 0,   0, 0,  //
      'L', 'L', 0, 0,  //
      'D', 0,   0, 0,  //
      'L', 'R', 0, 0,  //
      'C', 0,   0, 0,  //
  };
  return kDirectionAcronyms;
}

const char* XYDirectionString(int direction) {
  if (direction < 0 || direction >= kXYCenter) direction = kXYCenter;
  return &XYDirectionAcronyms()[direction << 2];  //< << 2 to * 4.
}

void XYDirectionHistoryPrint(int bits) {
  const char* dir_strings = XYDirectionAcronyms();
  for (int i = 0; i < 8; ++i) {
    if ((bits >> i) & 1) {
      Print(&XYDirectionAcronyms()[i]);
      if (i != 7) Print('\n', ' ');
    }
  }
}

const char** Maze1(int& height) {
  static const char* kMaze[] = {
      "         +--+",  //
      "+--------+  |",  //
      "| @         |",  //
      "| *+-- --+  |",  //
      "|  |     |  |",  //
      "|  |     |  |",  //
      "|  | +-+ |  |",  //
      "|  |X| | |  |",  //
      "|  +-+ | |  |",  //
      "|      | |  |",  //
      "|  +---+ |  |",  //
      "|  |        |",  //
      "|  |        |",  //
      "|  +-----   |",  //
      "|           |",  //
      "+-----------+",  //
  };
  height = 16;
  return kMaze;
}

char* PreprocessMaze(MazeLoader get_maze, int& width, int& height) {
  cout << "\n1.) Load the maze from a GetMaze function.";
  int y;
  const char** strings = get_maze(y);
  const char* read = strings[0];
  int x = 0;
  while (*read++) ++x;
  cout << "\n  width:" << x << " height:" << y << '\n';
  if (!x || !y) {
    cout << "\nERROR: width and/or height can't be zero!";
    return nullptr;
  }
  width = x;
  height = y;

  char *tiles = new char[x * y], *cursor = tiles;
  cout << "\n2.) Packing strings into maze of size " << x * y << ".\n";
  for (int j = 0; j < y; ++j) {
    cout << '\n';
    read = strings[j];
    for (int k = 0; k < x; ++k) {
      char c = (char)ToUpper(*read++);
      cout << c;
      *cursor++ = c;
    }
  }
  cout << '\n';
  return tiles;
}

Maze::Maze(MazeLoader get_maze, int number) : number(number) {
  tiles = PreprocessMaze(get_maze, width, height);
}

Maze::Maze(char* tiles, int width, int height, int number)
    : tiles(tiles), width(width), height(height), number(number) {}

Maze::~Maze() {
  if (tiles) delete[] tiles;
}

bool Maze::IsValid() {
  return tiles == nullptr && width > 0 && height > 0 && number > 0;
}

char* Maze::Get(int x, int y) {
  if (x < 0 || y < 0 || x >= height || y >= height) return 0;
  return &tiles[y * width + x];
}

bool Maze::Set(int x, int y, char c) {
  if (x < 0 || y < 0 || x >= height || y >= height) return false;
  tiles[y * width + x] = c;
  return true;
}

bool Maze::Move(int x, int y, int new_x, int new_y) {
  int l_width = width;
  if (x < 0 || x >= l_width || y < 0 || y >= height || new_x < 0 ||
      new_x >= l_width || new_y < 0 || new_y >= height)
    return false;

  char* read = &tiles[y * width + x];
  char c = *read;
  *read = ' ';
  char* write = &tiles[new_y * width + new_x];
  *write = c;
  return true;
}

bool Maze::CanMove(int x, int y) { return (*Get(x, y)) == ' '; }

bool Maze::IsEnd(int x, int y) {
  char tile = *Get(x, y);
  return tile == 'F';
}

bool Maze::Find(int& x, int& y, char c) {
  if (c >= 'a' && c <= 'z') c -= 'z' - 'Z';
  char* cursor = tiles;
  for (int j = 0; j < height; ++j) {
    for (int k = 0; k < width; ++k) {
      char d = *cursor++;
      if (c == d) {
        x = k;
        y = j;
        *(--cursor) = ' ';  //< Remove the char.
        return true;
      }
    }
  }
  x = y = -1;
  return false;
}

bool Maze::FindStart(int& x, int& y, int& dx, int& dy) {
  if (!Find(x, y, '@')) return false;
  if (!Find(dx, dy, '*')) return false;
  dx = x - dx;
  dy = y - dy;
  if (dx < -1 || dx > 1 || dy < -1 || dy > 1) return false;
  return true;
}

bool Maze::FindFinish(int& x, int& y) { return Find(x, y, 'F'); }

void Maze::Print(int iteration) {
  cout << "\nMaze " << number << "\nIteration:" << iteration
       << " width:" << width << " height:" << height;
  char* cursor = tiles;
  for (int y = 0; y < height; ++y) {
    cout << '\n';
    for (int x = 0; x < width; ++x) {
      char c = *cursor++;
      if (!c) break;
      cout << c;
    }
  }
}

int XYVector(int direction, int& dy) {
  switch (direction) {
    case kXYRight: {
      dy = 0;
      return 1;
    }
    case kXYUpperRight: {
      dy = -1;
      return 0;
    }
    case kXYUp: {
      dy = 0;
      return 0;
    }
    case kXYUpperLeft: {
      dy = -1;
      return -1;
    }
    case kXYLeft: {
      dy = 0;
      return -1;
    }
    case kXYLowerLeft: {
      dy = 1;
      return -1;
    }
    case kXYDown: {
      dy = 1;
      return 0;
    }
    case kXYLowerRight: {
      dy = 1;
      return 1;
    }
  }
  dy = 0;
  return 0;
}

int XYDirection(int dx, int dy) {
  if (dx == 0) return dy < 0 ? kXYUp : kXYDown;
  if (dy == 0) return dx < 0 ? kXYLeft : kXYRight;
  if (dx < 0) return dy < 0 ? kXYLowerLeft : kXYUpperLeft;
  return dy < 0 ? kXYLowerRight : kXYUpperRight;
}

MazeAgent::State::State() {}

MazeAgent::State::State(const State& other) {
  state = other.state;
  x = other.x;
  y = other.y;
  a = other.a;
  speed_y = other.speed_y;
  speed_a = other.speed_a;
  history = other.history;
}

void MazeAgent::State::Set(int new_x, int new_y, int new_a, int new_speed_a,
                           int new_speed_y, int new_state, int new_history) {
  state = new_state;
  x = new_x;
  y = new_y;
  a = new_a;
  speed_y = new_speed_y;
  speed_a = new_speed_a;
  history = new_history;
}

bool MazeAgent::State::Contains(int other_x, int other_y) {
  return x == other_x && y == other_y;
}

const char* MazeAgent::State::Label(int state) {
  switch (state) {
    case 0:  // kShutdown:
      return "Shutting down";
    case 1:  // kBooting:
      return "Booting";
    case kWaiting:
      return "Waiting";
    case kTurning:
      return "Turning";
    case kMoving:
      return "Moving";
    case kStuck:
      return "Stuck";
  }
  return "Error";
}

const char* MazeAgent::State::Label() { return Label(state); }

void MazeAgent::State::Print() {
  cout << "\nState: " << Label() << " x:" << x << " y:" << y << " a:" << a
       << " speed_y:" << speed_y << " speed_a:" << speed_a
       << "\nNodes traversed: ";
  XYDirectionHistoryPrint(history);
}

MazeAgent::Point2D::Point2D() {}

MazeAgent::Point2D::Point2D(State& state) : x(state.x), y(state.y) {}

MazeAgent::Point2D::Point2D(const Point2D& other) : x(other.x), y(other.y) {}

MazeAgent::MazeAgent(PolicyPilot pilot, MazeLoader get_maze, int maze_number)
    : state(),
      pilot(pilot),
      autopilot(pilot),
      stack_height(0),
      iteration(0),
      shortest_height(kStackSize),
      maze(get_maze, maze_number) {
  int x, y, dx, dy;
  maze.FindStart(x, y, dx, dy);
  state.Set(x, y, dx, dy, XYDirection(dx, dy));
}

bool MazeAgent::IsValid() { return maze.IsValid(); }

bool MazeAgent::ChangeState(int new_state) {
  int l_stack_height = stack_height;
  if (l_stack_height >= kStackSize) return false;

  switch (new_state) {
    case State::kWaiting: {
      state.speed_a = -1;
      state.speed_y = 0;
      return true;
    }
  }
  return false;
}

bool MazeAgent::Move(int x, int y, int new_x, int new_y) {
  if (!maze.Set(x, y, ' ') || !maze.Set(new_x, new_y, 'S')) return false;
  maze.Print(++iteration);
  return true;
}

bool MazeAgent::IsLoop(int new_x, int new_y) {
  for (int i = stack_height - 1; i > 0; --i) {
    if (states[i].Contains(new_x, new_y)) {
      return true;
    }
  }
  return false;
}

int MazeAgent::Update() {
  switch (state.state) {
    case State::kShutdown: {
      return State::kShutdown;
    }
    case State::kBooting: {
      return State::kWaiting;
    }
    case State::kWaiting: {
      return State::kWaiting;
    }
    case State::kTurning: {
      return State::kTurning;
    }
    case State::kMoving: {
      return State::kMoving;
    }
    case State::kStuck: {
      return State::kStuck;
    }
    case State::kError: {
      return State::kError;
    }
  }
  int l_state = pilot(this);
  ChangeState(l_state);
  return l_state;
}

void MazeAgent::Turn() {
  int l_a = state.a + state.speed_a;
  if (l_a < 0)
    l_a = kXYLowerRight;
  else if (l_a > kXYLowerRight)
    l_a = kXYRight;

  int dy, dx = XYVector(l_a, dy), x = state.x, y = state.y, new_x = x + dx,
          new_y = y + dy;
  if (!maze.CanMove(new_x, new_y)) {
    state.speed_a = 0;
    return;
  }
  maze.Move(x, y, new_x, new_y);
}

void MazeAgent::TurnLeft() { state.speed_a = -1; }

void MazeAgent::TurnRight() { state.speed_a = 1; }

void MazeAgent::AutopilotToggle() {
  PolicyPilot manual_pilot = PolicyPilotManual;
  pilot = (pilot == manual_pilot) ? autopilot : manual_pilot;
}

void MazeAgent::Decelerate() {
  int l_speed_y = state.speed_y;
  if (l_speed_y < 0) return;
  state.speed_y = l_speed_y - 1;
}

void MazeAgent::Accellerate() {
  int l_speed = state.speed_y;
  if (l_speed > 0) return;
  state.speed_y = l_speed + 1;
}

void MazeAgent::ShutDown() { state.state = 0; }  // State::kShutdown; }

void MazeAgent::SetAutopilot(PolicyPilot new_pilot) {
  ASSERT(new_pilot);
  autopilot = new_pilot;
}

bool MazeAgent::IsOnAutopilot() { return pilot != PolicyPilotManual; }

void MazeAgent::Print() {
  ::_::Print("\nAgent:");
  if (pilot == PolicyPilotManual) {
    ::_::Print(" Autopilot.");
    if (pilot == PolicyPilotDepthFirstRoundRobin) {
      ::_::Print("Depth-first Round-robin.");
    } else {
      ::_::Print("unknown.");
    }
  } else {
    ::_::Print(" Puppet.");
  }
  ::_::Print("\n state:");
  ::_::Print(state.Label());
  ::_::Print();
}

int PolicyPilotManual(MazeAgent* agent) {
  return 0;  // MazeAgent::State::kShutdown;
}  //< namespace _

int PolicyPilotDepthFirstRoundRobin(MazeAgent* agent) {
  // PEAS: Performance Measurement, Environment,
  // Actuators,
  //       Sensors
  int l_state = agent->state.state;
  if (l_state <= MazeAgent::State::kWaiting) return l_state;

  if (l_state == MazeAgent::State::kWaiting) return l_state;

  int x = agent->state.x, y = agent->state.y;

  if (agent->maze.IsEnd(x, y)) {
    if (agent->stack_height < agent->shortest_height) {
      agent->shortest_height = agent->stack_height;
      return MazeAgent::State::kWaiting;
      // You need to how copy the state stack
      // somewhere and so you compare this path
      // to the others to find the shortest_path.
    }
  }
  // Measurement: N/A because all movements are
  // in units of 1. Environment:
  int dx = agent->state.speed_a, dy = agent->state.speed_y;
  if (agent->maze.CanMove(x + dx, y + dy)) {
    // Keep going the same direction until we hit
    // a wall.
    agent->Move(x, y, x + dx, y + dy);
    return l_state;
  }
  return ++l_state;
}

}  //< namespace _
