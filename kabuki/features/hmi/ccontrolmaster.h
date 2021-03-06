/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_master_controls.h
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
#ifndef HEADER_FOR_KT_HMI_MASTERCONTROLS
#define HEADER_FOR_KT_HMI_MASTERCONTROLS

#include "parameter.h"

namespace _ {

/* Master control for an isymmetric control surface.
 */
class ControlMaster {
 public:
  /* Constructs a blank set of master controls. */
  ControlMaster();

  /* Prints this object to a terminal. */
  inline Utf& Print(Utf& printer) const;

 private:
  Parameter<int>*a, *b, *c, *d;
};

}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MASTERCONTROLS
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
