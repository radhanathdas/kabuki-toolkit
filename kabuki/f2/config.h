/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    \kabuki\f2\config.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once

#ifndef INCLUDED_KABUKI_F2_CONFIG
#define INCLUDED_KABUKI_F2_CONFIG 1

#define API

#include <cstdarg>
#include <cstdint>

typedef unsigned int uint;
typedef int32_t Tms;
typedef int64_t Tme;
typedef int type_t;
typedef unsigned int uint_t;
typedef unsigned char char_t;
typedef signed char schar_t;

#if COMPILER == VISUAL_CPP
#define FORMAT_SI8 "%I64i"
#define FORMAT_UI8 "%I64u"
#else
#define FORMAT_SI8 "%lld"
#define FORMAT_UI8 "%llu"
#endif

#if PLATFORM == WINDOWS
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
#else
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
#endif

#define SEAM_0_0_0__00 1   //< kabuki.f2.itos_and_stoi
#define SEAM_0_0_0__01 2   //< kabuki.f2.ftos_and_stof
#define SEAM_0_0_0__02 3   //< kabuki.f2.core_text_functions
#define SEAM_0_0_0__03 4   //< kabuki.f2.ascii_strings
#define SEAM_0_0_0__04 5   //< kabuki.f2.ascii_clock
#define SEAM_0_0_0__05 6   //< kabuki.f2.ascii_stack
#define SEAM_0_0_0__06 7   //< kabuki.f2.ascii_array
#define SEAM_0_0_0__07 8   //< kabuki.f2.ascii_loom
#define SEAM_0_0_0__08 9   //< kabuki.f2.ascii_table
#define SEAM_0_0_0__09 10  //< kabuki.f2.ascii_varint
#define SEAM_0_0_0__10 11  //< kabuki.f2.ascii_list
#define SEAM_0_0_0__11 12  //< kabuki.f2.ascii_map
#define SEAM_0_0_0__12 13  //< kabuki.f2.ascii_book
#define SEAM_0_0_0__13 14  //< kabuki.f2.ascii_dictionary
#endif                     //< #ifndef INCLUDED_KABUKI_F2_CONFIG
