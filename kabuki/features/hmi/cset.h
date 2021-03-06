/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_set.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_SET
#define HEADER_FOR_KT_HMI_SET

#include "../pro/project.h"

namespace _ {

/* A collection of shared objects, . */
class API Set {
 public:
  /* Constructor. */
  Set();

  /* Copy constructor copies the other object. */
  Set(const Set& s);

  /* Destructor. */
  virtual ~Set();

  /* Adds a new Project to the set. */
  void Add(Project& project);

  /* Gets the number of projects in the set. */
  int GetNumProjects();

  /* Prints this object to the terminal. */
  void Print();

 private:
  _::Array<Project&> projects;  //< Array of Project(s).
};                              //< class Set
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_SET
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
