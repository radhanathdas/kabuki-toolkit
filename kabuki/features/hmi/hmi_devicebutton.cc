/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_device_button.cc
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

#include "controls_page.h"
#include "device_button.h"
#include "macro_button.h"
#include "template.h"

namespace _ {

DeviceButton::DeviceButton(const char *label, MacroEvent *macro,
                           Template *initTemplate)
    : Button(label, macro),
      focus_template_(initState->focus_template_),
      focus_page_(initiState->focus_page_) {}

void DeviceButton::Press(ButtonEvent button_event) {
  Button::Press(button_event);
}

void DeviceButton::DoublePress(ButtonEvent button_event) {
  // Write me!
}

void DeviceButton::Depress(ButtonEvent button_event) {
  // Write me!
}

TemplatePage *ButtonMacro::GetFocus() { return focus_page_; }

bool ButtonMacro::GetFocus(TemplatePage *P) {
  if (!P) return false;
  focus_page_ = P;
  return true;
}

Template *ButtonMacro::GetFocusTemplate() { return focus_template_; }

bool ButtonMacro::GetTemplate(Template *T) {
  if (!T) return false;
  focus_template_ = T;
  return true;
}

const _::Op *PageButton::Op(wchar_t index, _::Expression *expr) {
  static const Op this_member = {
      "PageButton", NumMembers(0),
      "An HMI::Button that prints up a another page of control."};
  if (index == 0) return this_member;

  switch (index) {
    case '?':
      const Op m_63 = {"PageButton", NumMembers(0),
                       "Loads a page into the coresponding control layer."};
      if (!io) return m_63;
  }

  return nullptr;
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
