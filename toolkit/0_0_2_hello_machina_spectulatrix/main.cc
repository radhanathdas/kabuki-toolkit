/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    //seam_0_9/0_9_0_test/main.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "maze_agent.h"
using namespace _;

int main(int args_count, char** args) {
  Print("\nPreprocessing Maze...\n");
  int width, height, page_columns = 25;
  char* tiles = PreprocessMaze(Maze1, width, height);

  Print("\nTesting Maze...\n");
  Maze maze(tiles, width, height, 1);

  Print("\nCreating Agent...\n");
  MazeAgent agent;
  agent.Print();

  Pause("\nWould you like to edit the default keyboard layout?");

  Print(
      "\nEntering maze.\nControls: A/D moves left/right and W/D moves "
      "forward/stop/backwards.\nPress E to toggle autopilot on/off.\n\n");
  do {  // Main loop just scans for keyboard input.
    for (int i = page_columns; i > 0; --i) Print();
    // Get keyboard input for WASD, Quit, and Pause.
    if (GetAsyncKeyState(VK_)) int key = ToUpper(CInKey());
    switch (key) {
      case 'A': {
        agent.TurnLeft();
        break;
      }
      case 'D': {
        agent.TurnRight();
        break;
      }
      case 'E': {
        agent.AutopilotToggle();
        break;
      }
      case 'P': {
        for (int i = 0xfffffff; i > 0; --i)
          ;
        break;
      }
      case 'Q': {
        agent.ShutDown();
        break;
      }
      case 'S': {
        agent.Decelerate();
        break;
      }
      case 'W': {
        agent.Accellerate();
        break;
      }
    }
  } while (agent.Update());
  Print("\n\nShutdown...");
  return 0;
}
