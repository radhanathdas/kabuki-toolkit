/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/seams/0_0_seam_tests/test_seam_0_0_0.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#include <random>
#include "../../kabuki/f2/global.h"

#if SEAM == SEAM_0_0_0
namespace _ {

const char* TestSEAM_0_0_0 {
  Print("\n    Testing SEAM_0_0... ");

  static const uint64_t k10ToThe[20] = {
      1,           //< 10^0
      10,          //< 10^1
      100,         //< 10^2
      1000,        //< 10^3
      10000,       //< 10^4
      100000,      //< 10^5
      1000000,     //< 10^6
      10000000,    //< 10^7
      100000000,   //< 10^8
      1000000000,  //< 10^9
  };

  static const uint64_t test_value[] = {
      1,
      12,
      123,
      1234,
      12345,
      123456,
      1234567,
      12345678,
      123456789,
      k10ToThe[0] - 1,
      k10ToThe[0],
      k10ToThe[0] + 1,
      k10ToThe[1] - 1,
      k10ToThe[1],
      k10ToThe[1] + 1,
      k10ToThe[1] + k10ToThe[1] / 2,
      k10ToThe[2] - 1,
      k10ToThe[2],
      k10ToThe[2] + 1,
      k10ToThe[2] + k10ToThe[2] / 2,
      k10ToThe[3] - 1,
      k10ToThe[3],
      k10ToThe[3] + 1,
      k10ToThe[3] + k10ToThe[3] / 2,
      k10ToThe[4] - 1,
      k10ToThe[4],
      k10ToThe[4] + 1,
      k10ToThe[4] + k10ToThe[4] / 2,
      k10ToThe[5] - 1,
      k10ToThe[5],
      k10ToThe[5] + 1,
      k10ToThe[5] + k10ToThe[5] / 2,
      k10ToThe[6] - 1,
      k10ToThe[6],
      k10ToThe[6] + 1,
      k10ToThe[6] + k10ToThe[6] / 2,
      k10ToThe[7] - 1,
      k10ToThe[7],
      k10ToThe[7] + 1,
      k10ToThe[7] + k10ToThe[7] / 2,
      k10ToThe[8] - 1,
      k10ToThe[8],
      k10ToThe[8] + 1,
      k10ToThe[8] + k10ToThe[8] / 2,
      k10ToThe[9] - 1,
      k10ToThe[9],
      k10ToThe[9] + 1,
      k10ToThe[9] + k10ToThe[9] / 2,
  };

  /*
  Printf ("\nTesting quick MSD lookup...\n    Length 1:");
  static const uint64_t delta_one_bits[] = { 4, 7, 10, 14, 17, 20, 24, 27, 30 };
  uint64_t max,
           num_bits,
           msd_bit_range;
  Printf ("\n| Length | MSD Offset | Min Value  |"
          "\n|:------:|:----------:|:----------:|");
  for (int length = 3; length < 10; ++length) {
      num_bits = delta_one_bits[length - 2];
      msd_bit_range = (length == 10 || length == 20) ? 16: 8;
      for (uint64_t i = 1;  i <= 8; ++i) {
          Printf ("\n|   %llu    |     %llu      | %10u |", length, i, i <<
  num_bits);
      }
  }
  Print ('\n');
  system ("Pause");*/

  static const uint64_t problem_children[] = {
      0,
  };
  enum { kNumProblemChildren = 0, kSize = 24 };

  char text[kSize + 1], expecting[kSize + 1];
  char* result;
  uint64_t value;

  // Setup C++1x random number generator.
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint64_t> distr;

  Printf("\nTesting ScanUnsigned<Char, I> (const Char*, const char*, I);");

  for (int i = 0; i < 1 << 6; ++i) {
    uint64_t result, value = distr(eng);
    char buffer[25];
    Print<>(buffer, buffer + 24, value);
    Assert(Scan(buffer, result));
    Compare(value, result);
  }

  Printf("\n\nTesting Script ItoS Algorithm...\n\n");

  // PrintDigits99To99Lut ();

  Printf("\n\nTesting problem children...\n\n");
  for (int i = 0; i < kNumProblemChildren; ++i) {
    value = problem_children[i];
    sprintf_s(expecting, 24, "%llu", value);
    Printf("\n%i.) Expecting \"%s\":%llu", i + 1, expecting,
           (uint)strlen(expecting));
    result = Print<>(text, text + kSize - 1, value);
    if (!result) {
      Pause("An error occurred :-(");
      break;
    }
    *result = 0;
    if (strcmp(expecting, text)) {
      Printf("\n\nERROR: Expecting \"%s\":%llu and found \"%s\":%llu",
             expecting, (uint)strlen(expecting), text, (uint)strlen(text));
      Pause("\n");
    }
  }

  Print("\n\nTesting edge conditions...\n\n");
  for (int i = 0; i < 28; ++i) {
    value = test_value[i];
    sprintf_s(expecting, 24, "%llu", value);
    Printf("\n%i.) ", i + 1);
    result = Print<>(text, text + kSize - 1, value);
    if (!result) {
      Pause("An error occurred :-(");
      break;
    }
    *result = 0;
    if (strcmp(expecting, text)) {
      Printf("\n\nERROR: Expecting \"%s\":%llu and found \"%s\":%llu",
             expecting, (uint)strlen(expecting), text, (uint)strlen(text));
      Pause("\n");
    }
  }

  Printf("\n\nTesting random numbers...\n\n");

  for (int i = 0; i < 0x0000ffff; ++i) {
    value = distr(eng);
    sprintf_s(expecting, 24, "%llu", value);
    result = Print<>(text, text + kSize - 1, value);
    if (!result) {
      Pause("An error occurred :-(");
      break;
    }
    *result = 0;
    if (strcmp(expecting, text)) {
      Printf("\n\nERROR: Expecting \"%s\":%llu and found \"%s\":%llu",
             expecting, (uint)strlen(expecting), text, (uint)strlen(text));
      Pause("\n");
    }
  }

  Printf("\n Done testing ItoS :)\n\n");
}

}  // namespace _
#endif  //< #if SEAM == SEAM_0_0_0