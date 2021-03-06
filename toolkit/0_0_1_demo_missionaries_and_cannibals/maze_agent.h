/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/seam_0_0/0_0_0_experiments/maze_agent.h
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

#ifndef INCLUDED_KABUKI_AI_MAZEAGENT
#define INCLUDED_KABUKI_AI_MAZEAGENT 1

namespace _ {

//------------------------------------------ F2 --------------------------------

void Print(const char* string);
void Print(char c = '\n');
void Print(char c, char d);
void Print(const char* string, int value);
void Print(int value);
int ToLower(int c);
int ToUpper(int c);
int CInKey();
bool CInState(int vk_code);
void Pause(const char* message = nullptr);
bool Assert(bool condition);
bool AssertHandle(const char* file, int line);

#define ASSERT(condition) \
  if (Assert(condition)) _::AssertHandle(__FILE__, __LINE__)

//---------------------------------------- End F2 ------------------------------

// Function pointer for getting static mazes and their height.
typedef const char** (*MazeLoader)(int& height);

// Example maze with function pointer.
const char** Maze1(int& height);

char* PreprocessMaze(MazeLoader get_maze, int& width, int& height);

/*
# Allowed Moves
Our ASCII robot car takes up 2 characters that cannot be diagonal. */
struct Maze {
  char* tiles;
  int width, height, number;

  Maze(MazeLoader get_maze = Maze1, int number = 1);
  Maze(char* tiles, int width, int height, int number);
  bool IsValid();
  ~Maze();
  char* Get(int x, int y);
  bool Set(int x, int y, char c);
  bool Move(int x, int y, int new_x, int new_y);
  bool CanMove(int x, int y);
  bool IsEnd(int x, int y);
  bool Find(int& x, int& y, char c);
  bool FindStart(int& x, int& y, int& dx, int& dy);
  bool FindFinish(int& x, int& y);
  void Print(int iteration = 0);
};

enum {
  kXYRight = 0,   //< Direction 0: Right @ 0 degrees.
  kXYUpperRight,  //< Direction 1: Upper Right @ 45 degrees.
  kXYUp,          //< Direction 2: Up @ 90 degrees.
  kXYUpperLeft,   //< Direction 3: Upper Left @ 135 degrees.
  kXYLeft,        //< Direction 4: Left @ 180 degrees.
  kXYLowerLeft,   //< Direction 5: Lower Left @ 225 degrees.
  kXYDown,        //< Direction 6: Down @ 270 degrees.
  kXYLowerRight,  //< Direction 7: Lower Right @ 315 degrees.
  kXYCenter,      //< Direction 7: Center @ i degrees.
};

const char* XYDirectionAcronyms();

/* Decodes an XY quadrant packed in +/0/- format.
@param quad The quadrant.
@param value If value < 0 then only the lower angle node has been traversed. If
value == 0 then only the higher value node has been traversed. If value > 0 then
both nodes have been traversed. */
const char* XYDirectionString(int direction);

const char* XYDirectionAcronyms();

void XYDirectionHistoryPrint(int bits);

/* Gets the dx and dy values for the given direction.
@return DeltaX.
@param  a  The a-axis.
@param  dy DeltaY. */
int XYVector(int a, int& dy);

/* Gets the direction based on in the dx and dy.
@return A direction.
@param  dx Delta x.
@param  dy Delta y. */
int XYDirection(int dx, int dy);

struct MazeAgent;

typedef int (*PolicyPilot)(MazeAgent* agent);

int PolicyPilotManual(MazeAgent* agent);

int PolicyPilotDepthFirstRoundRobin(MazeAgent* agent);

/* Simple stack AI agent for solving simple mazes with 8 directions.
In order to better pack the data, the visited directions in the stack search
are that each quadrant contains two paths where quad_n is zero if neither path
has been traversed, it's -1 if the lower value angle path has been visited and
+1 if both nodes have been visited.
Speed will always be either -1, 0, or +1, and direction will always be
*/
struct MazeAgent {
  enum { kStackSize = 1024 };

  struct State {
    enum {
      kShutdown = 0,  //< State 0: Shutting Down.
      kBooting,       //< State 1: booting.
      kWaiting,       //< Not doing anything.
      kTurning,       //< Turning.
      kMoving,        //< Moving without stepping.
      kStuck,         //< Stuck on this path.
      kError,         //< Can't find a solution.
    };

    static const char* Label(int state);

    int state,    //< State variable.
        x,        //< X-axis position.
        y,        //< Y-axis position.
        a,        //< A-axis position.
        speed_y,  //< Speed in the Y-axis.
        speed_a,  //< Speed in the X-axis.
        history;  //< History of which of the 8
    State();
    State(const State& other);
    void Set(int new_x, int new_y, int new_a, int new_speed_a = 0,
             int new_speed_y = 0, int new_state = kBooting,
             int new_history = 0);
    const char* Label();
    void Print();
    bool Contains(int other_x, int other_y);
  };

  struct Point2D {
    int x, y;
    Point2D();
    Point2D(State& state);
    Point2D(const Point2D& other);
  };

  PolicyPilot pilot, autopilot;
  State state, next_state, states[kStackSize];
  int stack_height, iteration, shortest_height, init_dx, init_dy;
  Maze maze;
  Point2D shortest_path[kStackSize];

  MazeAgent(PolicyPilot pilot = PolicyPilotManual, MazeLoader get_maze = Maze1,
            int maze_number = 1);
  bool IsValid();
  bool ChangeState(int new_state);
  bool Move(int x, int y, int new_x, int new_y);
  bool IsLoop(int new_x, int new_y);
  int Update();
  void Turn();
  void TurnLeft();
  void TurnRight();
  void AutopilotToggle();
  void Decelerate();
  void Accellerate();
  void ShutDown();
  void SetAutopilot(PolicyPilot new_pilot);
  bool IsOnAutopilot();
  void Print();
};

int PolicyPilotManual(MazeAgent* agent);
int PolicyPilotDepthFirstRoundRobin(MazeAgent* agent);

}  //< namespace _

#endif  //< #ifndef INCLUDED_KABUKI_AI_MAZEAGENT
