/** Kabuki Toolkit
    @file    ~/source/hmi/set.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KABUKI_TOOLKIT_HMI_SET
#define HEADER_FOR_KABUKI_TOOLKIT_HMI_SET

#include "../pro/project.h"

namespace kt { namespace hmi {

/** A collection of shared objects, . */
class KABUKI Set {
    public:

    /** Constructor. */
    Set ();
    
    /** Copy constructor copies the other object. */
    Set  (const Set& s);

    /** Destructor. */
    virtual ~Set ();

    /** Adds a new Project to the set. */
    void Add (Project& project);

    /** Gets the number of projects in the set. */
    int GetNumProjects ();

    /** Prints this object to the terminal. */
    void Print ();

    private:

    data::Array<Project&> projects;     //< Array of Project(s).
};      //< class Set
}       //< namespace hmi
}       //< namespace kt
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_HMI_SET
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1