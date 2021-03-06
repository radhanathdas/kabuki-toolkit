/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_key_event_buffer.h
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
#ifndef HEADER_FOR_KT_HMI_KEYEVENTBUFFER
#define HEADER_FOR_KT_HMI_KEYEVENTBUFFER

#include "config.h"

namespace _ {

/* A computer keyboard event buffer.
    Keyboard events are stored as delegates.
*/
class API KeyEventBuffer {
  enum {
    DefaultBufferSize = 256,   //< The default buffer size.
    MinBufferSize = 64,        //< Min buffer size.
    MaxBufferSize = 1024 * 10  //< Max buffer size.
  };

  /* Default constructor. */
  KeyEventBuffer(int a = DefaultBufferSize);

  /* Resets the event buffer. */
  void Clear();

  /* Inserts an event into the buffer. */
  void Insert(Event a);

  /*  */
  void Update();

 private:
  int begin, end, start, stop, bufferSize;
  Event* buffer;

};  //< class KeyEventBuffer
}  // namespace _
}  //< namespace kt
#endif  //< HEADER_FOR_KT_HMI_KEYEVENTBUFFER
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
