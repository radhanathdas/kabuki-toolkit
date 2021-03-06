/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_ButtonRadio.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "channel_group.h"

namespace _ {

ChannelGroup::ChannelGroup() {}

const byte* ChannelGroup::GetChannelAddress(uint_t channel) {
  if (channel > channels_.size()) return nullptr;
  return channels_[channel];
}

void ChannelGroup::SetChannelAddress(uint_t channel, const byte* address) {
  if (address == nullptr) return;
  size_t l_size = channels_.size();
  if (channel > l_size) return;
  if (channel == l_size) {
    channels_.push_back(address);
    return;
  }
  channels_[channel] = address;
}

uint_t ChannelGroup::AddChannel(const byte* address) {
  if (address == nullptr) return ~0;
  channels_.push_back(address);
  return channels_.size() - 1;
}

bool ChannelGroup::RemoveChannel(uint_t index) {
  if (index >= channels_.size()) return false;
  channels_.erase(channels_.begin() + index, channels_.begin() + index + 1);
}

void ChannelGroup::Print() const {
  size_t l_size = channels_.size();
  Printf("ChannelGroup: size=%u\n", l_size);

  for (size_t i = 0; i < l_size; ++i) {
    Printf("%9u: ");
    const byte* ch = channels_[i];
    char c;
    do {
      c = *ch;
      Printf("%u ", c);
      ++ch;
    } while (c != 0);
    PrintNL();
  }
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
