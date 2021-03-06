/** Kabuki Toolkit
    @file    /.../Source/_tek/utils/morse_code.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once
#include <pch.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef KABUKI_TEK_UTILS_MORSECODE_H
#define KABUKI_TEK_UTILS_MORSECODE_H

namespace _ {

const char* ToMorseCode (char value);
/*< Gets the Morse code for the given char. */

}       //< _utils
#endif  //< KABUKI_TEK_UTILS_MORSECODE_H
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
