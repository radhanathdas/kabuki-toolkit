/* Kabuki Toolkit @version 0.x
@file    ~/seam_00/00/test_crabs/seam.h
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

#include "header.h"

#include "00/seam.h"
#include "01/seam.h"

namespace _ {

static const char* Seam_00_00_08(char* seam_log, char* seam_end, 
                          const char* args) {
  if (!TestBegin(seam_log, seam_end, args)) return __FUNCTION__;
  return TestTreeNode<Seam_00_00_08__00>(seam_log, seam_end, args);
}
}  // namespace _
#include "footer.h"
