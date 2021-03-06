/* Kabuki Toolkit
<<<<<<< HEAD
    @file    /library/kt/id/role.h
=======
    @file    $kabuki-toolkit/library/kt/id/role.h
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once
#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_ROLE
#define HEADER_FOR_KT_ID_ROLE
#include "config.h"

namespace _ {

/* A role that an entity plays in an organization.
    Examples of a role are:
    

    1. Owner
    2. Executive
    3. Manager
    4. Employee
    5. Volunteer
*/
struct API Role {
  uid_t GetRole() = 0;
};
}       // namespace _
#endif  //< HEADER_FOR_KT_ID_ROLE
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
