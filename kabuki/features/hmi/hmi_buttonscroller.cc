/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_button_scroller.cc
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

#include "button_scroller.h"

namespace _ {

ButtonScroller::ButtonScroller(string initLabel, int newNumVisibleButtons)
    : ButtonScroller(initLabel), currentIndex(0), buttons() {
  setNumVisibleButtons(newNumVisibleButtons);
}

ButtonScroller::ButtonScroller(const ButtonScroller& o)
    : HmiComponent(o),
      numVisibleButtons(o.numVisibleButtons),
      currentIndex(o.currentIndex),
      buttons(o.buttons) {
  // Nothing to do here yet.
}

Button** ButtonScroller::GetButtons() { return &buttons[0]; }

int ButtonScroller::GetNumButtons() const { return buttons.size(); }

void ButtonScroller::SetNumVisibleButtons(int value) {
  numVisibleButtons = (value < 1) ? 1 : value;
}

void ButtonScroller::RemoveButton(int index) {
  int l_numButtons = getNumButtons();
  if (index < 0 || index >= l_numButtons) return;
  if (buttons.remove(index)) {
    ;
  }

  if (index > currentIndex) --currentIndex;

  if (currentIndex + numVisibleButtons >= l_numButtons) {
    currentIndex = l_numButtons - numVisibleButtons;
  }
}

int ButtonScroller::GetNumVisibleButtons() const { return numVisibleButtons; }

int ButtonScroller::GetCurrentIndex() const { return currentIndex; }

int ButtonScroller::press(int index) {
  if (index < 0) return -1;

  if (index >= numVisibleButtons) return 1;

  int buttonIndex = currentIndex + index;

  if (buttonIndex > getNumButtons()) return 2;

  auto temp = buttons[buttonIndex];
  temp->press();
  return 0;
}

void ButtonScroller::scrollUp() {
  // Note: Up equals getPrevious.

  if (currentIndex == 0)  // Special Case: We can't scroll any farther.
    return;

  --currentIndex;
}

void ButtonScroller::scrollDown() {
  if (currentIndex >=
      getNumButtons() + numVisibleButtons)  // We can't scroll any farther!
    return;

  ++currentIndex;
}

void ButtonScroller::scrollPageUp() { scrollUp(numVisibleButtons); }

void ButtonScroller::scrollPageDown() { scrollDown(numVisibleButtons); }

void ButtonScroller::scrollUp(int numTimes) {
  if (numTimes < 1) return;

  int thisNumButtons = getNumButtons();

  if (currentIndex + numTimes >
      thisNumButtons)  //< Were at the end and can't scroll any farther!
    currentIndex =
        thisNumButtons -
        numVisibleButtons;  //< Show the last numVisibleButtons Buttons.

  currentIndex -= numTimes;
}

void ButtonScroller::scrollDown(int numTimes) {
  int l_numButtons = getNumButtons();
  if (currentIndex + numTimes >=
      l_numButtons)  // Special Case: We can't scroll any farther.
    numTimes = l_numButtons - currentIndex - numVisibleButtons;

  currentIndex += numTimes;
}

void ButtonScroller::print() const {
  int i;

  Printf(
      "Controls:::ButtonScroller %s \n_numVisibleButtons: %i, size (): "
      "\nVisible Buttons: ",
      getLabel(), numVisibleButtons, getNumButtons());

  for (i = 0; i < numVisibleButtons; ++i) {
    if (buttons[i] == nullptr) {
      Printf("\n");
      break;
    }

    Printf(buttons[i]->getLabel());

    if (i != numVisibleButtons - 1)
      Printf(" -> ");
    else
      Printf("\n");
  }

  if (i == 0) Printf("Empty\n");
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
