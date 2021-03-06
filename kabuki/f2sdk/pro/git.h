/* Kabuki Toolkit
    @version 0.x
    @file    /projects/seam_2/kabuki_toolkit_updater/git.h
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM >= SEAM_0_0_0__07
#include "config.h"

#ifndef HEADER_FOR_KABUKI_PRO_GIT
#define HEADER_FOR_KABUKI_PRO_GIT

namespace _ {

/** Script wrapper class for scanning git console commands.
Currently supported functions:
* git clone TKN
*/
class Git : public Operand {
 public:
  /* Constructs nothing. */
  Git();

  /* Script operations. */
  virtual const Op* Star(wchar_t index, Expr* expr);
};
}  // namespace _
#endif  //< HEADER_FOR_KABUKI_PRO_GIT
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
