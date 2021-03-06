/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/typecraft_project.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "../../../kabuki-toolkit/source/kabuki/crabs/public.h"
#include "../../../kabuki-toolkit/source/kabuki/pro/include/project.h"

using namespace _::pro;
using namespace std;

class TestProject : public Project {
 public:
  TestProject() : Project() {}

  void ProjectLog() {
    BeginLog("2017-10-22@02:00");
    BeginHeading("Project Logging System with Script");
    Log("This is an experimental new logging system with Script using the "
        "operator+ "
        "The idea behind it is simple: We want to integrate the issue tracking "
        "system "
        "with the source code files. We just need a way to track issues and "
        "route "
        "messages to people."
        ""
        "This module works with the _::id module and _::app module to");
    EndLog();
  }
};
