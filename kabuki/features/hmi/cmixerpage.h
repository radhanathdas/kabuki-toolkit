/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/mixer_page.h
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
#ifndef HEADER_FOR_KT_HMI_MIXERPAGE
#define HEADER_FOR_KT_HMI_MIXERPAGE

namespace _ {

class API Mixer;
class API MixerChannel;

/* Class that represents a page of mixer channels on the Symmetry Live!
    A MixerPage stores 8 TemplatePages in an array. The user can switch through
   banks of pages on the mixer. The mixer can either be sorted in ascending
   track index order or the page banks can be customized to make buses.
*/
class API MixerPage {
 public:
  /* Constructor. */
  MixerPage(Mixer* theMixer = nullptr, int initStartIndex = 0);

  /* Destructor. */
  ~MixerPage();

  /* Gets the startingIndex of this MixerPage. */
  int GetStartIndex();

  /* Sets the startingIndex to the newIndex. */
  void SetStartIndex(int newIndex);

  /* Replaces thisChannel with theNewOne. */
  void SetChannel(int thisChannel, MixerChannel* theNewOne);

  /* Gets a pointer to the channel[atThisIndex]. */
  MixerChannel* GetChannel(int index);

  /* Prints this object to a terminal. */
  _::Utf& Print(_::Utf& print);

 private:
  int starting_index_;  //< Starting index of this page this MixerPage.
  Mixer* mixer_;        //< Pointer to the global kabuki::Mixer.
  _::Array<MixerChannel*>
      channels_;  //< Array of pointers to pointers to MixerChannel(s).

};  //< class MixerPage
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_MIXERPAGE
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
