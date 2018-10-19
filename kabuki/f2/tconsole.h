/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/tconsole.h
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

#ifndef INCLUDED_KABUKI_F2_TCONSOLE
#define INCLUDED_KABUKI_F2_TCONSOLE 1

#include "console.h"

#include "tbinary.h"

namespace _ {

/* Prints the following value to the console in Hex. */
template <typename UI>
void PrintHex(UI value) {
  enum { kHexStringLengthSizeMax = sizeof(UI) * 2 + 3 };

  Print('0', 'x');
  for (int num_bits_shift = sizeof(UI) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4)
    Print(HexNibbleToUpperCase((uint8_t)(value >> num_bits_shift)));
}

}  // namespace _

#endif  //< INCLUDED_KABUKI_F2_TCONSOLE
